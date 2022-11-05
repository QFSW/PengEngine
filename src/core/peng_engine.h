#pragma once

#include <threading/worker_thread.h>

class PengEngine
{
public:
	PengEngine();

	void start();
	void request_shutdown();

	void set_target_fps(double fps) noexcept;
	void set_target_frametime(double frametime_ms) noexcept;

	bool shutting_down() const;

private:
	void start_opengl();
	void shutdown();
	void shutdown_opengl();

	void tick_main();
	void tick_render();
	void tick_opengl();

	bool _executing;
	double _target_frametime;

	struct GLFWwindow* _glfw_window;
	threading::WorkerThread _render_thread;
};