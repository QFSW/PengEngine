#include "peng_engine.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utils/timing.h>

PengEngine::PengEngine()
	: _executing(false)
	, _target_frametime(1000 / 60.0)
	, _last_frametime(_target_frametime)
	, _last_main_frametime(0)
	, _last_render_frametime(0)
	, _last_opengl_frametime(0)
	, _last_draw_time(timing::clock::now())
	, _glfw_window(nullptr)
	, _render_thread("RenderThread")
{ }

PengEngine& PengEngine::get()
{
	static PengEngine peng_engine;
	return peng_engine;
}

void PengEngine::start()
{
	_executing = true;
	start_opengl();

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

EntityManager& PengEngine::entity_manager() noexcept
{
	return _entity_manager;
}

void PengEngine::start_opengl()
{
	constexpr GLint width = 800;
	constexpr GLint height = 600;

	if (!glfwInit())
	{
		printf("GLFW initialization failed\n");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RED_BITS,   8);
    glfwWindowHint(GLFW_BLUE_BITS,  8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

	_glfw_window = glfwCreateWindow(width, height, "PengEngine", nullptr, nullptr);
	if (!_glfw_window)
	{
		printf("GLFW window creation failed\n");
		return;
	}

	int32_t buffer_width;
	int32_t buffer_height;
	glfwGetFramebufferSize(_glfw_window, &buffer_width, &buffer_height);
	glfwMakeContextCurrent(_glfw_window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed\n");
		return;
	}

	glViewport(0, 0, buffer_width, buffer_height);
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
	_last_main_frametime = timing::measure_ms([this] {
		const double delta_time = _last_frametime / 1000.0;
		
		_on_frame_start();
		_entity_manager.tick(delta_time);
	});
}

void PengEngine::tick_render()
{
	_last_render_frametime = timing::measure_ms([this] {

	});
}

void PengEngine::tick_opengl()
{
	_last_opengl_frametime = timing::measure_ms([this] {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		if (glfwGetKey(_glfw_window, GLFW_KEY_1) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (glfwGetKey(_glfw_window, GLFW_KEY_2) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
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
