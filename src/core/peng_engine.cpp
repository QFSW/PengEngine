#include "peng_engine.h"

#include <stdio.h>

PengEngine::PengEngine()
	: _render_thread("RenderThread")
{ }

void PengEngine::start()
{
	while (true)
	{
		tick_main();
	}
}

void PengEngine::tick_main()
{
	printf("Ticking main thread\n");

	// TODO: Move render ticking to its own thread
	tick_render();
}

void PengEngine::tick_render()
{
}
