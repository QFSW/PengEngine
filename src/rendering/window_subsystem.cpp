#include "window_subsystem.h"

#include <stdexcept>

#pragma warning( push, 0 )
#define NOMINMAX
#include <windows.h>
#pragma warning( pop )

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <core/logger.h>
#include <profiling/scoped_event.h>
#include <profiling/scoped_gpu_event.h>

// Causes the NVIDIA GPU to be used over integrated graphics on dual GPU systems (such as laptops)
// https://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

using namespace rendering;

WindowSubsystem::WindowSubsystem()
	: Subsystem()
	, _resolution(800, 600)
	, _windowed_resolution(_resolution)
	, _windowed_position(100, 100)
	, _fullscreen(false)
	, _cursor_locked(false)
	, _vsync(false)
	, _msaa_samples(0)
	, _window_name("PengEngine")
	, _window(nullptr)
	, _active(false)
    , _last_draw_time(timing::clock::now())
{ }

WindowSubsystem::~WindowSubsystem()
{
	if (_active)
	{
		shutdown();
	}
}

static void APIENTRY handle_gl_debug_output(
	GLenum, GLenum type, GLuint id, GLenum /*severity*/, GLsizei /*length*/, const char* message, const void* /*userParam*/
)
{
	// Filter out some overly verbose and not helpful debug output
	// Typically driver implementations logging about how something will be used
	switch (id)
	{
	    case 0x20071:
		case 0x20084: return;
		default: break;
	}

	switch (type)
	{
	    case GL_DEBUG_TYPE_ERROR:
	    {
		    Logger::error("OpenGL Error [%x]: %s", id, message);
		    break;
	    }
	    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	    {
		    Logger::error("OpenGL Deprecation [%x]: %s", id, message);
		    break;
	    }
	    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
	    {
		    Logger::error("OpenGL UB [%x]: %s", id, message);
		    break;
	    }
	    case GL_DEBUG_TYPE_PORTABILITY:
	    {
		    Logger::warning("OpenGL Portability [%x]: %s", id, message);
		    break;
	    }
	    case GL_DEBUG_TYPE_PERFORMANCE:
	    {
		    Logger::warning("OpenGL Performance [%x]: %s", id, message);
		    break;
	    }
	    case GL_DEBUG_TYPE_PUSH_GROUP:
	    case GL_DEBUG_TYPE_POP_GROUP:
	    {
		    break;
	    }
	    default:
	    {
		    Logger::log("OpenGL [%x]: %s", id, message);
		    break;
	    }
	}
}

void WindowSubsystem::start()
{
	SCOPED_EVENT("WindowSubsystem - start");
	Logger::log("Starting GLFW");

	check(!_active);
	_active = true;

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

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_SAMPLES, static_cast<GLint>(_msaa_samples));

	GLFWmonitor* monitor = _fullscreen ? glfwGetPrimaryMonitor() : nullptr;
	_window = glfwCreateWindow(_resolution.x, _resolution.y, _window_name.c_str(), monitor, nullptr);

	if (!_window)
	{
		const char* err_msg;
		int err_code = glfwGetError(&err_msg);

		throw std::logic_error(strtools::catf(
			"GLFW window creation failed (%d) - %s",
			err_code, err_msg
		));
	}

	glfwGetFramebufferSize(_window, &_resolution.x, &_resolution.y);
	glfwMakeContextCurrent(_window);

	Logger::log("OpenGL context created - %s", glGetString(GL_VERSION));
	Logger::log("GPU selected - %s (%s)",
		strtools::trim(reinterpret_cast<const char*>(glGetString(GL_RENDERER))).c_str(),
		strtools::trim(reinterpret_cast<const char*>(glGetString(GL_VENDOR))).c_str()
	);

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
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow*, int32_t width, int32_t height)
	{
		get()._resolution = math::Vector2i(width, height);
		glViewport(0, 0, width, height);
	});

	if (_msaa_samples > 0)
	{
		glEnable(GL_MULTISAMPLE);
	}

	glfwSetInputMode(_window, GLFW_CURSOR, _cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	glfwSwapInterval(_vsync ? 1 : 0);
}

void WindowSubsystem::shutdown()
{
	SCOPED_EVENT("WindowSubsystem - shutdown");
	Logger::log("Shutting down GLFW");

	check(_active);
	_active = false;

	if (_window)
	{
		glfwDestroyWindow(_window);
		_window = nullptr;
	}

	glfwTerminate();
}

void WindowSubsystem::tick(float)
{
	SCOPED_EVENT("WindowSubsystem - tick");

	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowSubsystem::finalize_frame(float target_frametime)
{
	SCOPED_EVENT("PengEngine - finalize frame");

	const timing::clock::time_point sync_point =
		_last_draw_time
		+ std::chrono::duration_cast<timing::clock::duration>(timing::duration_ms(target_frametime));

	timing::sleep_until_precise(sync_point);

	_last_draw_time = sync_point;

	SCOPED_GPU_EVENT("Finalize Frame");
	glfwSwapBuffers(_window);
}

void WindowSubsystem::set_resolution(const math::Vector2i& resolution) noexcept
{
	set_resolution(resolution, _fullscreen);
}

void WindowSubsystem::set_resolution(const math::Vector2i& resolution, bool fullscreen) noexcept
{
	// Cache the windowed resolution and position on fullscreen enter so we can restore it
	if (fullscreen && !_fullscreen)
	{
		_windowed_resolution = _resolution;
		glfwGetWindowPos(_window, &_windowed_position.x, &_windowed_position.y);
	}

	_resolution = resolution;
	_fullscreen = fullscreen;

	if (_active)
	{
		GLFWmonitor* monitor = _fullscreen ? glfwGetPrimaryMonitor() : nullptr;
		const math::Vector2i position = _fullscreen ? math::Vector2i::zero() : _windowed_position;

		glfwSetWindowSize(_window, _resolution.x, _resolution.y);
		glfwSetWindowMonitor(_window, monitor, position.x, position.y, _resolution.x, _resolution.y, GLFW_DONT_CARE);
	}
}

void WindowSubsystem::set_cursor_locked(bool cursor_locked)
{
	if (cursor_locked == _cursor_locked)
	{
		return;
	}

	_cursor_locked = cursor_locked;

	if (_active)
	{
		glfwSetInputMode(_window, GLFW_CURSOR, _cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}
}

void WindowSubsystem::set_vsync(bool vsync)
{
	_vsync = vsync;

	if (_active)
	{
		glfwSwapInterval(_vsync ? 1 : 0);
	}
}

void WindowSubsystem::set_msaa(uint32_t msaa_samples)
{
	if (msaa_samples == _msaa_samples)
	{
		return;
	}

	if (_active)
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

void WindowSubsystem::set_window_name(const std::string& name)
{
	_window_name = name;

	if (_active)
	{
		glfwSetWindowTitle(_window, _window_name.c_str());
	}
}

void WindowSubsystem::enter_fullscreen()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	set_resolution(math::Vector2i(mode->width, mode->height), true);
}

void WindowSubsystem::exit_fullscreen()
{
	set_resolution(_windowed_resolution, false);
}

void WindowSubsystem::toggle_fullscreen()
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

void WindowSubsystem::maximize_window() const
{
	if (!_active)
	{
		Logger::error("Cannot maximize window while the engine isn't running");
		return;
	}

	glfwMaximizeWindow(_window);
}

bool WindowSubsystem::should_close() const noexcept
{
	return _window && glfwWindowShouldClose(_window);
}

const math::Vector2i& WindowSubsystem::resolution() const noexcept
{
	return _resolution;
}

float WindowSubsystem::aspect_ratio() const noexcept
{
	const math::Vector2f resolution_f = _resolution;
	return resolution_f.x / resolution_f.y;
}

bool WindowSubsystem::fullscreen() const noexcept
{
	return _fullscreen;
}

GLFWwindow* WindowSubsystem::window_handle() const noexcept
{
	return _window;
}