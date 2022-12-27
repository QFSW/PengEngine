#include "peng_engine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utils/timing.h>

#include "logger.h"

PengEngine::PengEngine()
	: _executing(false)
	, _target_frametime(1000 / 60.0)
	, _resolution(800, 600)
	, _last_frametime(_target_frametime)
	, _last_main_frametime(0)
	, _last_render_frametime(0)
	, _last_opengl_frametime(0)
	, _last_draw_time(timing::clock::now())
	, _glfw_window(nullptr)
{ }

PengEngine& PengEngine::get()
{
	static PengEngine peng_engine;
	return peng_engine;
}

void PengEngine::start()
{
	_executing = true;
	Logger::get().log(LogSeverity::log, "PengEngine starting...");

	start_opengl();
	_input_manager.start(_glfw_window);

	Logger::get().log(LogSeverity::success, "PengEngine started");
	_on_engine_initialized();

	while (!shutting_down())
	{
		_last_frametime = timing::measure_ms([this] {
			tick_opengl();
			tick_main();
			tick_render();
			finalize_frame();
		});
	}

	Logger::get().log(LogSeverity::log, "PengEngine shutting down...");
	shutdown();
}

void PengEngine::request_shutdown()
{
	Logger::get().log(LogSeverity::log, "PengEngine shutdown requested");
	_executing = false;
}

void PengEngine::set_target_fps(double fps) noexcept
{
	set_target_frametime(1000 / fps);
}

void PengEngine::set_target_frametime(double frametime_ms) noexcept
{
	_target_frametime = frametime_ms;
}

void PengEngine::set_resolution(const math::Vector2i& resolution) noexcept
{
	if (_executing)
	{
		Logger::get().log(LogSeverity::error, "Changing the resolution after starting PengEngine is not yet supported");
		return;
	}

	_resolution = resolution;
}

bool PengEngine::shutting_down() const
{
	if (!_executing)
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

EntityManager& PengEngine::entity_manager() noexcept
{
	return _entity_manager;
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
			Logger::get().logf(LogSeverity::error, "OpenGL Error: %s", message);
			break;
		}
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		{
			Logger::get().logf(LogSeverity::warning, "OpenGL Deprecation: %s", message);
			break;
		}
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		{
			Logger::get().logf(LogSeverity::error, "OpenGL UB: %s", message);
			break;
		}
		case GL_DEBUG_TYPE_PORTABILITY:
		{
			Logger::get().logf(LogSeverity::warning, "OpenGL Portability: %s", message);
			break;
		}
		case GL_DEBUG_TYPE_PERFORMANCE:
		{
			Logger::get().logf(LogSeverity::warning, "OpenGL Performance: %s", message);
			break;
		}
		default:
		{
			Logger::get().logf(LogSeverity::log, "OpenGL: %s", message);
			break;
		}
	}
}

void PengEngine::start_opengl()
{
	if (!glfwInit())
	{
		printf("GLFW initialization failed\n");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if constexpr (Logger::enabled())
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}

	glfwWindowHint(GLFW_RED_BITS,   8);
    glfwWindowHint(GLFW_BLUE_BITS,  8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

	_glfw_window = glfwCreateWindow(_resolution.x, _resolution.y, "PengEngine", nullptr, nullptr);
	if (!_glfw_window)
	{
		throw std::logic_error("GLFW window creation failed");
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

	glfwGetFramebufferSize(_glfw_window, &_resolution.x, &_resolution.y);
	glfwMakeContextCurrent(_glfw_window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::logic_error("GLEW initialization failed");
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, _resolution.x, _resolution.y);
	glfwSetFramebufferSizeCallback(_glfw_window, [](GLFWwindow* window, int32_t width, int32_t height)
	{
		get()._resolution = math::Vector2i(width, height);
		glViewport(0, 0, width, height);
	});
}

void PengEngine::shutdown()
{
	_entity_manager.shutdown();
	shutdown_opengl();

	Logger::get().log(LogSeverity::success, "PengEngine shutdown");
}

void PengEngine::shutdown_opengl()
{
	if (_glfw_window)
	{
		glfwDestroyWindow(_glfw_window);
		_glfw_window = nullptr;
	}

	glfwTerminate();
}

void PengEngine::tick_main()
{
	_last_main_frametime = timing::measure_ms([this] {
		const double delta_time = _last_frametime / 1000.0;
		
		_on_frame_start();

		_input_manager.tick();
		_entity_manager.tick(delta_time);

		_on_frame_end();
	});
}

void PengEngine::tick_render()
{
	_last_render_frametime = timing::measure_ms([this] {
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
	});
}

void PengEngine::tick_opengl()
{
	_last_opengl_frametime = timing::measure_ms([this] {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});
}

void PengEngine::finalize_frame()
{
	const timing::clock::time_point sync_point = 
		_last_draw_time 
		+ std::chrono::duration_cast<timing::clock::duration>(timing::duration_ms(_target_frametime));

	timing::sleep_until_precise(sync_point);

	_last_draw_time = sync_point;
	glfwSwapBuffers(_glfw_window);
}
