#include "peng_engine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utils/timing.h>
#include <rendering/render_queue.h>
#include <audio/audio_subsystem.h>
#include <profiling/scoped_event.h>
#include <profiling/scoped_gpu_event.h>

#include "logger.h"
#include "entity_subsystem.h"

PengEngine::PengEngine()
	: Singleton()
	, _executing(false)
	, _shutting_down(false)
	, _target_frametime(1000 / 60.0f)
	, _max_delta_time(0)
	, _resolution(800, 600)
	, _windowed_resolution(_resolution)
	, _windowed_position(100, 100)
	, _fullscreen(false)
	, _cursor_locked(false)
	, _vsync(false)
	, _msaa_samples(0)
	, _frame_number(0)
	, _last_frametime(_target_frametime)
	, _last_draw_time(timing::clock::now())
	, _window_name("PengEngine")
	, _glfw_window(nullptr)
{
	Subsystem::load<EntitySubsystem>();
	Subsystem::load<audio::AudioSubsystem>();
}

void PengEngine::run()
{
	start();

	while (!shutting_down())
	{
		_last_frametime = static_cast<float>(timing::measure_ms([this] {
			tick();
		}));
	}

	Logger::log("PengEngine shutting down...");
	shutdown();
}

void PengEngine::request_shutdown()
{
	Logger::log("PengEngine shutdown requested");
	_shutting_down = true;
}

void PengEngine::set_target_fps(float fps) noexcept
{
	set_target_frametime(1000 / fps);
}

void PengEngine::set_target_frametime(float frametime_ms) noexcept
{
	_target_frametime = frametime_ms;
}

void PengEngine::set_max_delta_time(float frametime_ms) noexcept
{
	_max_delta_time = frametime_ms;
}

void PengEngine::set_resolution(const math::Vector2i& resolution) noexcept
{
	set_resolution(resolution, _fullscreen);
}

void PengEngine::set_resolution(const math::Vector2i& resolution, bool fullscreen) noexcept
{
	// Cache the windowed resolution and position on fullscreen enter so we can restore it
	if (fullscreen && !_fullscreen)
	{
		_windowed_resolution = _resolution;
		glfwGetWindowPos(_glfw_window, &_windowed_position.x, &_windowed_position.y);
	}

	_resolution = resolution;
	_fullscreen = fullscreen;

	if (_executing)
	{
		GLFWmonitor* monitor = _fullscreen ? glfwGetPrimaryMonitor() : nullptr;
		const math::Vector2i position = _fullscreen ? math::Vector2i::zero() : _windowed_position;

		glfwSetWindowSize(_glfw_window, _resolution.x, _resolution.y);
		glfwSetWindowMonitor(_glfw_window, monitor, position.x, position.y, _resolution.x, _resolution.y, GLFW_DONT_CARE);
	}
}

void PengEngine::set_cursor_locked(bool cursor_locked)
{
	if (cursor_locked == _cursor_locked)
	{
		return;
	}

	_cursor_locked = cursor_locked;

	if (_executing)
	{
		glfwSetInputMode(_glfw_window, GLFW_CURSOR, _cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}
}

void PengEngine::set_vsync(bool vsync)
{
	_vsync = vsync;

	if (_executing)
	{
		glfwSwapInterval(_vsync ? 1 : 0);
	}
}

void PengEngine::set_msaa(uint32_t msaa_samples)
{
	if (msaa_samples == _msaa_samples)
	{
		return;
	}

	if (_executing)
	{
		Logger::error("Changing MSAA at runtime is not yet supported");
		return;
	}

	if ((msaa_samples & (msaa_samples - 1)) != 0)
	{
		Logger::error("Cannot set MSAA to %dx as %d is not a power of 2", msaa_samples, msaa_samples);
		return;
	}

	_msaa_samples = msaa_samples;
}

void PengEngine::set_window_name(const std::string& name)
{
	_window_name = name;

	if (_executing)
	{
		glfwSetWindowTitle(_glfw_window, _window_name.c_str());
	}
}

void PengEngine::enter_fullscreen()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	set_resolution(math::Vector2i(mode->width, mode->height), true);
}

void PengEngine::exit_fullscreen()
{
	set_resolution(_windowed_resolution, false);
}

void PengEngine::toggle_fullscreen()
{
	if (_fullscreen)
	{
		exit_fullscreen();
	}
	else
	{
		enter_fullscreen();
	}
}

void PengEngine::maximize_window() const
{
	if (!_executing)
	{
		Logger::error("Cannot maximize window while the engine isn't running");
		return;
	}

	glfwMaximizeWindow(_glfw_window);
}

bool PengEngine::shutting_down() const
{
	if (_shutting_down)
	{
		return true;
	}

	if (_glfw_window && glfwWindowShouldClose(_glfw_window))
	{
		return true;
	}

	return false;
}

const math::Vector2i& PengEngine::resolution() const noexcept
{
	return _resolution;
}

float PengEngine::aspect_ratio() const noexcept
{
	const math::Vector2f resolution_f = _resolution;
	return resolution_f.x / resolution_f.y;
}

bool PengEngine::fullscreen() const noexcept
{
	return _fullscreen;
}

input::InputManager& PengEngine::input_manager() noexcept
{
	return _input_manager;
}

static void APIENTRY handle_gl_debug_output(GLenum, GLenum type, unsigned int, GLenum, GLsizei, const char* message, const void*)
{
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:
		{
			Logger::error("OpenGL Error: %s", message);
			break;
		}
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		{
			Logger::error("OpenGL Deprecation: %s", message);
			break;
		}
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		{
			Logger::error("OpenGL UB: %s", message);
			break;
		}
		case GL_DEBUG_TYPE_PORTABILITY:
		{
			Logger::warning("OpenGL Portability: %s", message);
			break;
		}
		case GL_DEBUG_TYPE_PERFORMANCE:
		{
			Logger::warning("OpenGL Performance: %s", message);
			break;
		}
		default:
		{
			Logger::log("OpenGL: %s", message);
			break;
		}
	}
}

void PengEngine::start()
{
	SCOPED_EVENT("PengEngine - start");

	_executing = true;
	Logger::log("PengEngine starting...");

	start_opengl();
	_input_manager.start(_glfw_window);
	Subsystem::start_all();

	Logger::success("PengEngine started");
	_on_engine_initialized();
}

void PengEngine::start_opengl()
{
	if (!glfwInit())
	{
		throw std::logic_error("GLFW initialization failed");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef PENG_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, Logger::enabled());
#endif

	glfwWindowHint(GLFW_RED_BITS,   8);
    glfwWindowHint(GLFW_BLUE_BITS,  8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_SAMPLES, static_cast<GLint>(_msaa_samples));

	GLFWmonitor* monitor = _fullscreen ? glfwGetPrimaryMonitor() : nullptr;
	_glfw_window = glfwCreateWindow(_resolution.x, _resolution.y, _window_name.c_str(), monitor, nullptr);

	if (!_glfw_window)
	{
		throw std::logic_error("GLFW window creation failed");
	}

	glfwGetFramebufferSize(_glfw_window, &_resolution.x, &_resolution.y);
	glfwMakeContextCurrent(_glfw_window);

	Logger::log("OpenGL context created - %s", glGetString(GL_VERSION));

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::logic_error("GLEW initialization failed");
	}

	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glDebugMessageCallback(handle_gl_debug_output, nullptr);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, _resolution.x, _resolution.y);
	glfwSetFramebufferSizeCallback(_glfw_window, [](GLFWwindow*, int32_t width, int32_t height)
	{
		get()._resolution = math::Vector2i(width, height);
		glViewport(0, 0, width, height);
	});

	if (_msaa_samples > 0)
	{
		glEnable(GL_MULTISAMPLE);
	}

	glfwSetInputMode(_glfw_window, GLFW_CURSOR, _cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	glfwSwapInterval(_vsync ? 1 : 0);
}

void PengEngine::shutdown()
{
	SCOPED_EVENT("PengEngine - shutdown");

	Subsystem::shutdown_all();
	shutdown_opengl();

	_shutting_down = false;
	_executing = false;

	Logger::success("PengEngine shutdown");
}

void PengEngine::shutdown_opengl()
{
	SCOPED_EVENT("PengEngine - shutdown opengl");

	if (_glfw_window)
	{
		glfwDestroyWindow(_glfw_window);
		_glfw_window = nullptr;
	}

	glfwTerminate();
}

void PengEngine::tick()
{
	SCOPED_EVENT("Frame", strtools::catf_temp("%d", _frame_number), {255, 200, 255});

	tick_opengl();
	tick_main();
	tick_render();
	finalize_frame();

	_frame_number++;
}

void PengEngine::tick_main()
{
	SCOPED_EVENT("PengEngine - tick main");

	const float frametime_capped =
		_max_delta_time > 0
		? std::min(_last_frametime, _max_delta_time)
		: _last_frametime;

	const float delta_time = frametime_capped / 1000.0f;
	
	_on_frame_start();

	_input_manager.tick();
	EntitySubsystem::get().tick(delta_time);

	_on_frame_end();
}

void PengEngine::tick_render()
{
	SCOPED_EVENT("PengEngine - tick render");

#ifndef PENG_MASTER
	if (_input_manager[input::KeyCode::num_row_1].pressed())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (_input_manager[input::KeyCode::num_row_2].pressed())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (_input_manager[input::KeyCode::num_row_3].pressed())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
#endif

	rendering::RenderQueue::get().execute();
}

void PengEngine::tick_opengl()
{
	SCOPED_EVENT("PengEngine - tick opengl");

	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PengEngine::finalize_frame()
{
	SCOPED_EVENT("PengEngine - finalize frame");

	const timing::clock::time_point sync_point = 
		_last_draw_time 
		+ std::chrono::duration_cast<timing::clock::duration>(timing::duration_ms(_target_frametime));

	timing::sleep_until_precise(sync_point);

	_last_draw_time = sync_point;

	SCOPED_GPU_EVENT("Finalize Frame");
	glfwSwapBuffers(_glfw_window);
}
