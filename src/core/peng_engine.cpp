#include "peng_engine.h"

#include <GL/glew.h>

#include <utils/timing.h>
#include <memory/gc.h>
#include <rendering/render_queue.h>
#include <rendering/window_subsystem.h>
#include <audio/audio_subsystem.h>
#include <input/input_subsystem.h>
#include <profiling/scoped_event.h>

#include "logger.h"
#include "entity_subsystem.h"

PengEngine::PengEngine()
	: Singleton()
	, _executing(false)
	, _shutting_down(false)
	, _target_frametime(1000 / 60.0f)
	, _max_delta_time(0)
    , _time_scale(1)
	, _frame_number(0)
	, _last_frametime(_target_frametime)
	, _frametime_smoothing_window(100)
	, _previous_frametimes_total()
{
	Subsystem::load<rendering::WindowSubsystem>();
	Subsystem::load<audio::AudioSubsystem>();
	Subsystem::load<input::InputSubsystem>();
	Subsystem::load<EntitySubsystem>();

	_previous_frametimes.reserve(_frametime_smoothing_window);
}

void PengEngine::run()
{
	start();

	while (!shutting_down())
	{
		record_frametime(static_cast<float>(timing::measure_ms([this] {
			tick();
		})));
	}

	Logger::log("PengEngine shutting down...");
	shutdown();
}

void PengEngine::request_shutdown()
{
	Logger::log("PengEngine shutdown requested");
	_shutting_down = true;
}

void PengEngine::set_target_fps(float fps) noexcept
{
	set_target_frametime(1000 / fps);
}

void PengEngine::set_target_frametime(float frametime_ms) noexcept
{
	_target_frametime = frametime_ms;
}

void PengEngine::set_max_delta_time(float frametime_ms) noexcept
{
	_max_delta_time = frametime_ms;
}

void PengEngine::set_time_scale(float time_scale) noexcept
{
	check(time_scale >= 0);
	_time_scale = time_scale;
}

bool PengEngine::shutting_down() const
{
	if (_shutting_down)
	{
		return true;
	}

	if (rendering::WindowSubsystem::get().should_close())
	{
		return true;
	}

	return false;
}

float PengEngine::time_scale() const noexcept
{
	return _time_scale;
}

int32_t PengEngine::frame_number() const noexcept
{
	return _frame_number;
}

float PengEngine::last_frametime() const noexcept
{
	return _last_frametime;
}

float PengEngine::last_frametime_smoothed() const noexcept
{
	return _previous_frametimes_total / _frametime_smoothing_window;
}

void PengEngine::start()
{
	SCOPED_EVENT("PengEngine - start");

	_executing = true;
	Logger::log("PengEngine starting...");

	Subsystem::start_all();

	Logger::success("PengEngine started");
	_on_engine_initialized();
}

void PengEngine::shutdown()
{
	SCOPED_EVENT("PengEngine - shutdown");

	Subsystem::shutdown_all();

	_shutting_down = false;
	_executing = false;

	Logger::success("PengEngine shutdown");
}

void PengEngine::tick()
{
	SCOPED_EVENT("Frame", strtools::catf_temp("%d", _frame_number), {255, 200, 255});

	tick_main();
	tick_render();

	memory::GC::get().tick();
	rendering::WindowSubsystem::get().finalize_frame(_target_frametime);

	_frame_number++;
}

void PengEngine::tick_main()
{
	SCOPED_EVENT("PengEngine - tick main");

	const float frametime_capped =
		_max_delta_time > 0
		? std::min(_last_frametime * _time_scale, _max_delta_time)
		: _last_frametime;

	const float delta_time = frametime_capped / 1000.0f;
	
	_on_frame_start();

	Subsystem::tick_all(delta_time);

	_on_frame_end();
}

void PengEngine::tick_render()
{
	SCOPED_EVENT("PengEngine - tick render");

#ifndef PENG_MASTER
	if (input::InputSubsystem::get()[input::KeyCode::num_row_1].pressed())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (input::InputSubsystem::get()[input::KeyCode::num_row_2].pressed())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (input::InputSubsystem::get()[input::KeyCode::num_row_3].pressed())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
#endif

	rendering::RenderQueue::get().execute();
}

void PengEngine::record_frametime(float frametime)
{
	_last_frametime = frametime;

	// Pop oldest frametime if we're at window capacity
	if (_previous_frametimes.size() == _frametime_smoothing_window)
	{
		_previous_frametimes_total -= _previous_frametimes[0];
		_previous_frametimes.erase(_previous_frametimes.begin());
	}

	// Push new frametime
	_previous_frametimes_total += _last_frametime;
	_previous_frametimes.push_back(_last_frametime);
}