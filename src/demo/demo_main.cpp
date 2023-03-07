#include "demo_main.h"

#include <fstream>

#include <core/peng_engine.h>
#include <core/entity_factory.h>
#include <profiling/profiler_manager.h>
#include <profiling/superluminal_profiler.h>
#include <libs/nlohmann/json.hpp>

namespace demo
{
    int demo_main()
    {
        profiling::ProfilerManager::get().load_profiler<profiling::SuperluminalProfiler>();

        std::ifstream level_file("resources/maps/demo/gravity.json");
        nlohmann::json level_definition = nlohmann::json::parse(level_file);

        if (const auto it = level_definition.find("entities"); it != level_definition.end())
        {
            if (it->is_array())
            {
                for (const auto& entity : *it)
                {
                    const std::string entity_type = entity["type"].get<std::string>();
                    const std::string entity_name = entity.contains("name") ? entity["name"].get<std::string>() : "";

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
            }
            else
            {
                Logger::error("The 'entities' entry in the level was not an array");
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