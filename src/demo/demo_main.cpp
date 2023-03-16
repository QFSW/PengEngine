#include "demo_main.h"

#include <core/peng_engine.h>
#include <profiling/profiler_manager.h>
#include <world/world_loader.h>

#ifndef NO_PROFILING
#include <profiling/superluminal_profiler.h>
#endif

namespace demo
{
    int demo_main()
    {
#ifndef NO_PROFILING
            profiling::ProfilerManager::get().load_profiler<profiling::SuperluminalProfiler>();
#endif

        PengEngine::get().on_engine_initialized().subscribe_once([]
        {
            world::WorldLoader world_loader;
            world_loader.load_from_file("resources/worlds/demo/gravity.json");
        });
        
        PengEngine::get().set_resolution(math::Vector2i(1280, 720));
        PengEngine::get().set_vsync(false);
        PengEngine::get().set_target_frametime(0);
        PengEngine::get().set_msaa(4);
        PengEngine::get().run();

        return 0;
    }
}