#include "peng_engine.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utils/timing.h>

PengEngine::PengEngine()
	: _target_frametime(1000 / 60.0)
	, _render_thread("RenderThread")
	, _glfw_window(nullptr)
{ }

void PengEngine::start()
{
	_executing = true;
	start_opengl();

	while (!shutting_down())
	{
		const double frametime = timing::measure_ms([this] {
			tick_main();
		});

		if (frametime < _target_frametime)
		{
			const double sleep_time = _target_frametime - frametime;
			timing::sleep_for_precise(timing::duration_ms(sleep_time));
		}

		printf("Frametime = %.02fms\n", frametime);
	}

	shutdown();
}

void PengEngine::request_shutdown()
{
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

void PengEngine::start_opengl()
{
	const GLint Width = 800;
	const GLint Height = 600;

	if (!glfwInit())
	{
		printf("GLFW initialization failed\n");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	_glfw_window = glfwCreateWindow(Width, Height, "PengEngine", nullptr, nullptr);
	if (!_glfw_window)
	{
		printf("GLFW window creation failed\n");
		return;
	}

	int32_t bufferWidth;
	int32_t bufferHeight;
	glfwGetFramebufferSize(_glfw_window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(_glfw_window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed\n");
		return;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);
}

void PengEngine::shutdown()
{
	shutdown_opengl();
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
	// TODO: Move render ticking to its own thread
	tick_render();
}

void PengEngine::tick_opengl()
{
	glfwPollEvents();

	glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(_glfw_window);
}

void PengEngine::tick_render()
{
	// TODO: we could pipeline the OpenGL tick as its own thread potentially
	tick_opengl();
}
