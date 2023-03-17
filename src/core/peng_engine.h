#pragma once

#include <math/vector2.h>
#include <input/input_manager.h>
#include <utils/timing.h>
#include <utils/event.h>
#include <utils/singleton.h>

#include "entity_manager.h"

// TODO: move opengl code into an RHI object
class PengEngine : public utils::Singleton<PengEngine>
{
	friend Singleton;

	DEFINE_EVENT(on_engine_initialized)
	DEFINE_EVENT(on_frame_start)
	DEFINE_EVENT(on_frame_end)

public:
	void run();
	void request_shutdown();

	void set_target_fps(float fps) noexcept;
	void set_target_frametime(float frametime_ms) noexcept;
	void set_max_delta_time(float frametime_ms) noexcept;
	void set_resolution(const math::Vector2i& resolution) noexcept;
	void set_resolution(const math::Vector2i& resolution, bool fullscreen) noexcept;
	void set_cursor_locked(bool cursor_locked);
	void set_vsync(bool vsync);
	void set_msaa(uint32_t msaa_samples);
	void set_window_name(const std::string& name);

	void maximize_window() const;

	[[nodiscard]] bool shutting_down() const;
	[[nodiscard]] const math::Vector2i& resolution() const noexcept;
	[[nodiscard]] float aspect_ratio() const noexcept;
	[[nodiscard]] bool fullscreen() const noexcept;

	EntityManager& entity_manager() noexcept;
	input::InputManager& input_manager() noexcept;

private:
	PengEngine();

	void start();
	void start_opengl();
	void shutdown();
	void shutdown_opengl();

	void tick();
	void tick_main();
	void tick_render();
	void tick_opengl();

	void finalize_frame();

	bool _executing;
	bool _shutting_down;
	float _target_frametime;
	float _max_delta_time;

	math::Vector2i _resolution;
	bool _fullscreen;
	bool _cursor_locked;
	bool _vsync;
	uint32_t _msaa_samples;

	int32_t _frame_number;
	float _last_frametime;
	timing::clock::time_point _last_draw_time;

	std::string _window_name;
	struct GLFWwindow* _glfw_window;

	EntityManager _entity_manager;
	input::InputManager _input_manager;
};