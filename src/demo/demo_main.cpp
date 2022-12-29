#include "demo_main.h"

#include <core/peng_engine.h>

#include "demo_controller.h"

namespace demo
{
    int demo_main()
    {
        PengEngine::get().entity_manager().create_entity<DemoController>();
        PengEngine::get().set_resolution(math::Vector2i(1280, 720));
        PengEngine::get().set_target_fps(60);
        PengEngine::get().start();

        return 0;
    }
}