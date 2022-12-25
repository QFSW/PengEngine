#pragma once

#include <math/vector2.h>
#include <threading/worker_thread.h>
#include <utils/timing.h>
#include <utils/event.h>

#include "entity_manager.h"

class PengEngine
{
	DEFINE_EVENT(on_engine_initialized);
	DEFINE_EVENT(on_frame_start);

public:
	static PengEngine& get();

	void start();
	void request_shutdown();

	void set_target_fps(double fps) noexcept;
	void set_target_frametime(double frametime_ms) noexcept;
	void set_resolution(const math::Vector2u& resolution) noexcept;

	bool shutting_down() const;
	const math::Vector2u& resolution() const noexcept;

	EntityManager& entity_manager() noexcept;

private:
	PengEngine();
	~PengEngine() = default;

	PengEngine(const PengEngine&) = delete;
	PengEngine(PengEngine&&) = delete;

	PengEngine& operator=(const PengEngine&) = delete;
	PengEngine& operator=(PengEngine&&) = delete;

	void start_opengl();
	void shutdown();
	void shutdown_opengl();

	void tick_main();
	void tick_render();
	void tick_opengl();

	void finalize_frame();

	bool _executing;
	double _target_frametime;

	math::Vector2u _resolution;

	double _last_frametime;
	double _last_main_frametime;
	double _last_render_frametime;
	double _last_opengl_frametime;
	timing::clock::time_point _last_draw_time;

	struct GLFWwindow* _glfw_window;
	threading::WorkerThread _render_thread;

	EntityManager _entity_manager;
};