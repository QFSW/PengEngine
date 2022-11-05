#pragma once

#include <threading/worker_thread.h>

class PengEngine
{
public:
	PengEngine();

	void start();

private:
	void tick_main();
	void tick_render();

	threading::WorkerThread _render_thread;
};