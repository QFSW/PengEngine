#include "demo_main.h"

#include <core/peng_engine.h>

#include "demo_controller.h"

namespace demo
{
    int demo_main()
    {
        PengEngine::get().entity_manager().create_entity<DemoController>();
        PengEngine::get().set_resolution(math::Vector2i(1280, 720));
        PengEngine::get().set_vsync(true);
        PengEngine::get().set_msaa(4);
        PengEngine::get().start();

        return 0;
    }
}