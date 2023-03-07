#include "world_loader.h"

#include <fstream>

#include <core/reflection_database.h>
#include <core/entity_factory.h>
#include <core/logger.h>
#include <profiling/scoped_event.h>

using namespace world;

void WorldLoader::load_from_file(const std::string& path)
{
    SCOPED_EVENT("WorldLoader - load from file");
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file " + path);
    }

    const nlohmann::json world_def = nlohmann::json::parse(file);
    load_from_json(world_def);
}

void WorldLoader::load_from_json(const nlohmann::json& world_def)
{
    SCOPED_EVENT("WorldLoader - load from json");
    load_entities(world_def);
}

void WorldLoader::load_entities(const nlohmann::json& world_def)
{
    if (const auto it = world_def.find("entities"); it != world_def.end())
    {
        if (it->is_array())
        {
            for (const auto& entity_def : *it)
            {
                load_entity(entity_def);
            }
        }
        else
        {
            Logger::error("The 'entities' entry in the world was not an array");
        }
    }
    else
    {
        Logger::warning("Loading world with no entities");
    }
}

void WorldLoader::load_entity(const nlohmann::json& entity_def)
{
    const std::string entity_type = get_value<std::string>(entity_def, "type");
    const std::string entity_name = get_value_or_default<std::string>(entity_def, "name");

    if (const auto reflected_type = ReflectionDatabase::get().reflect_type(entity_type))
    {
        peng::weak_ptr<Entity> entity = EntityFactory::get().create_entity(reflected_type.to_shared_ref(), entity_name);
    }
    else
    {
        Logger::error(
            "Could not load entity '%' as the type '%s' does not exist",
            entity_name.c_str(), entity_type.c_str()
        );
    }
}
