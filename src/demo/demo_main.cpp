#include "demo_main.h"

#include <core/peng_engine.h>
#include <core/entity_factory.h>
#include <profiling/profiler_manager.h>
#include <profiling/superluminal_profiler.h>
#include <utils/csv.h>

namespace demo
{
    int demo_main()
    {
        profiling::ProfilerManager::get().load_profiler<profiling::SuperluminalProfiler>();

        const auto entity_definitions = csv::read_file<std::tuple<std::string, std::string>>(
			"resources/maps/demo/demo.csv"
        );

        for (auto [entity_type, entity_name] : entity_definitions)
        {
            auto reflected_type = ReflectionDatabase::get().reflect_type(entity_type);
            if (reflected_type)
            {
                EntityFactory::get().create_entity(reflected_type.to_shared_ref(), entity_name);
            }
            else
            {
                Logger::error(
					"Could not load entity '%' as the type '%s' does not exist",
                    entity_name.c_str(), entity_type.c_str()
                );
            }
        }
        
        PengEngine::get().set_resolution(math::Vector2i(1280, 720));
        PengEngine::get().set_vsync(false);
        PengEngine::get().set_target_frametime(0);
        PengEngine::get().set_msaa(4);
        PengEngine::get().run();

        return 0;
    }
}