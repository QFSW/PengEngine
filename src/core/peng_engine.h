#pragma once

#include <threading/worker_thread.h>

class PengEngine
{
public:
	PengEngine();

	void start();
	void shutdown();

	void set_target_frametime(double frametime_ms) noexcept;

private:
	void tick_main();
	void tick_render();

	bool _executing;
	double _target_frametime;

	threading::WorkerThread _render_thread;
};