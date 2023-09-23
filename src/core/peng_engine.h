#pragma once

#include <math/vector2.h>
#include <utils/event.h>
#include <utils/singleton.h>

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
	void set_time_scale(float time_scale) noexcept;

	[[nodiscard]] bool shutting_down() const;
	[[nodiscard]] float time_scale() const noexcept;
	[[nodiscard]] int32_t frame_number() const noexcept;
	[[nodiscard]] float last_frametime() const noexcept;
	[[nodiscard]] float last_frametime_smoothed() const noexcept;

private:
	PengEngine();

	void start();
	void shutdown();

	void tick();
	void tick_main();
	void tick_render();

	void record_frametime(float frametime);

	bool _executing;
	bool _shutting_down;
	float _target_frametime;
	float _max_delta_time;
	float _time_scale;

	int32_t _frame_number;
	float _last_frametime;

	size_t _frametime_smoothing_window;
	std::vector<float> _previous_frametimes;
	float _previous_frametimes_total;
};