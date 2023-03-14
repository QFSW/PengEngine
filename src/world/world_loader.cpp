#include "world_loader.h"

#include <fstream>

#include <core/reflection_database.h>
#include <core/entity_factory.h>
#include <core/component_factory.h>
#include <core/logger.h>
#include <profiling/scoped_event.h>
#include <math/json_support.h>

#include "core/entity.h"

using namespace world;

void WorldLoader::load_from_file(const std::string& path)
{
    SCOPED_EVENT("WorldLoader - load from file");
    Logger::log("Loading world '%s'", path.c_str());

    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file " + path);
    }

    nlohmann::json world_def;

    try
    {
        world_def = nlohmann::json::parse(file);
    }
    catch (const nlohmann::json::parse_error& e)
    {
        Logger::error("Error occurred while parsing JSON - terminating world loading");
        Logger::error(e.what());
        return;
    }

    load_from_json(world_def);
    Logger::success("Loaded world '%s'", path.c_str());
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
        if (!it->is_array())
        {
	        Logger::error("The 'entities' entry in the world was not an array");
            return;
        }

        for (const auto& entity_def : *it)
        {
	        load_entity(entity_def);
        }
    }
    else
    {
        Logger::warning("Loading world with no entities");
    }
}

void WorldLoader::load_entity(const nlohmann::json& entity_def)
{
    // TODO: instead of crashing if no type is provided we should error out
    const std::string entity_type = get_value<std::string>(entity_def, "type");
    const std::string entity_name = get_value_or_default<std::string>(entity_def, "name");
    const math::Transform entity_transform = get_value_or_default<math::Transform>(entity_def, "transform");

    if (const auto reflected_type = ReflectionDatabase::get().reflect_type(entity_type))
    {
	    const peng::weak_ptr<Entity> entity = EntityFactory::get().create_entity(reflected_type.to_shared_ref(), entity_name);

        entity->local_transform() = entity_transform;
        load_components(entity_def, entity);
    }
    else
    {
        Logger::error(
            "Could not load entity '%s' as the type '%s' does not exist",
            entity_name.c_str(), entity_type.c_str()
        );
    }
}

void WorldLoader::load_components(const nlohmann::json& entity_def, const peng::weak_ptr<Entity>& entity)
{
    if (const auto it = entity_def.find("components"); it != entity_def.end())
    {
        if (!it->is_array())
        {
	        Logger::error("The 'components' entry in the entity '%s' was not an array", entity->name().c_str());
            return;
        }

        for (const auto& component_def : *it)
        {
	        load_component(component_def, entity);
        }
    }
}

void WorldLoader::load_component(const nlohmann::json& component_def, const peng::weak_ptr<Entity>& entity)
{
    if (!component_def.is_string())
    {
	    Logger::error(
		    "Could not load component '%s' on entity '%s' as it is not a component typename",
		    component_def.dump().c_str(), entity->name().c_str()
        );
        return;
    }

    const std::string component_type = component_def.get<std::string>();
    if (const auto reflected_type = ReflectionDatabase::get().reflect_type(component_type))
    {
        ComponentFactory::get().create_component(reflected_type.to_shared_ref(), entity);
    }
    else
    {
	    Logger::error(
		    "Could not add component '%s' to entity '%s' as the type does not exist",
		    component_type.c_str(), entity->name().c_str()
	    );
    }
}
