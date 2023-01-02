#pragma once

#include <math/vector2.h>
#include <input/input_manager.h>
#include <utils/timing.h>
#include <utils/event.h>

#include "entity_manager.h"

// TODO: move opengl code into an RHI object
class PengEngine
{
	DEFINE_EVENT(on_engine_initialized)
	DEFINE_EVENT(on_frame_start)
	DEFINE_EVENT(on_frame_end)

public:
	static PengEngine& get();

	void start();
	void request_shutdown();

	void set_target_fps(double fps) noexcept;
	void set_target_frametime(double frametime_ms) noexcept;
	void set_resolution(const math::Vector2i& resolution) noexcept;
	void set_cursor_locked(bool cursor_locked);

	[[nodiscard]] bool shutting_down() const;
	[[nodiscard]] const math::Vector2i& resolution() const noexcept;

	EntityManager& entity_manager() noexcept;
	input::InputManager& input_manager() noexcept;

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

	math::Vector2i _resolution;
	bool _cursor_locked;

	double _last_frametime;
	double _last_main_frametime;
	double _last_render_frametime;
	double _last_opengl_frametime;
	timing::clock::time_point _last_draw_time;

	struct GLFWwindow* _glfw_window;

	EntityManager _entity_manager;
	input::InputManager _input_manager;
};