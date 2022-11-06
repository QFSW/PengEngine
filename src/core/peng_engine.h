#pragma once

#include <threading/worker_thread.h>
#include <utils/timing.h>
#include <utils/event.h>

class PengEngine
{
public:
	PengEngine();

	void start();
	void request_shutdown();

	void set_target_fps(double fps) noexcept;
	void set_target_frametime(double frametime_ms) noexcept;

	bool shutting_down() const;

	utils::EventInterface<>& get_on_engine_initialized() noexcept;
	utils::EventInterface<>& get_on_frame_start() noexcept;

private:
	void start_opengl();
	void shutdown();
	void shutdown_opengl();

	void tick_main();
	void tick_render();
	void tick_opengl();

	void finalize_frame();

	bool _executing;
	double _target_frametime;

	double _last_frametime;
	double _last_main_frametime;
	double _last_render_frametime;
	double _last_opengl_frametime;
	timing::clock::time_point _last_draw_time;

	struct GLFWwindow* _glfw_window;
	threading::WorkerThread _render_thread;

	utils::Event<> _on_engine_initialized;
	utils::Event<> _on_frame_start;
};