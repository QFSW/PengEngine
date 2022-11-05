#include "peng_engine.h"

#include <stdio.h>
#include <chrono>

#include <utils/timing.h>

PengEngine::PengEngine()
	: _target_frametime(1000 / 60.0)
	, _render_thread("RenderThread")
{ }

void PengEngine::start()
{
	_executing = true;
	while (_executing)
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
}

void PengEngine::shutdown()
{
	_executing = false;
}

void PengEngine::set_target_frametime(double frametime_ms) noexcept
{
	_target_frametime = frametime_ms;
}

void PengEngine::tick_main()
{
	// TODO: Move render ticking to its own thread
	tick_render();
}

void PengEngine::tick_render()
{
}
