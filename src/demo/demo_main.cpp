#include "demo_main.h"

#include <core/peng_engine.h>
#include <core/asset.h>
#include <profiling/profiler_manager.h>
#include <scene/scene_loader.h>
#include <input/input_subsystem.h>
#include <rendering/window_subsystem.h>
#include <rendering/window_icon.h>
#include <entities/debug/bootloader.h>

#ifndef NO_PROFILING
#ifdef PLATFORM_WIN
#include <profiling/superluminal_profiler.h>
#endif
#endif

namespace demo
{
    int demo_main()
    {
#ifndef NO_PROFILING
#ifdef PLATFORM_WIN
            profiling::ProfilerManager::get().load_profiler<profiling::SuperluminalProfiler>();
#endif
#endif

        PengEngine::get().on_engine_initialized().subscribe_once([]
        {
            Asset<rendering::WindowIcon> icon("resources/textures/core/peng_engine_64.asset");
            icon.load()->use();

            scene::SceneLoader scene_loader;
            scene_loader.load_from_file("resources/scenes/demo/pong.json");
        });

#ifndef PENG_MASTER
        PengEngine::get().on_frame_start().subscribe([] {
            if (input::InputSubsystem::get()[input::KeyCode::f2].pressed())
            {
                entities::debug::Bootloader::initiate();
            }
        });
#endif
        
        rendering::WindowSubsystem::get().set_resolution(math::Vector2i(1280, 720));
        rendering::WindowSubsystem::get().set_vsync(false);
        PengEngine::get().set_target_frametime(0);
        PengEngine::get().run();

        return 0;
    }
}