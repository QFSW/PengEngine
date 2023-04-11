#include "scene_loader.h"

#include <fstream>

#include <core/archive.h>
#include <core/component.h>
#include <core/entity.h>
#include <core/reflection_database.h>
#include <core/entity_factory.h>
#include <core/component_factory.h>
#include <core/logger.h>
#include <profiling/scoped_event.h>

using namespace scene;

void SceneLoader::load_from_file(const std::string& path)
{
    SCOPED_EVENT("SceneLoader - load from file");
    Logger::log("Loading scene '%s'", path.c_str());

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
        Logger::error("Error occurred while parsing JSON - terminating scene loading");
        Logger::error(e.what());
        return;
    }

    load_from_json(world_def);
    Logger::success("Loaded scene '%s'", path.c_str());
}

void SceneLoader::load_from_json(const nlohmann::json& world_def)
{
    SCOPED_EVENT("SceneLoader - load from json");
    load_entities(world_def);
}

void SceneLoader::load_entities(const nlohmann::json& world_def)
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
            Archive archive;
            archive.json_def = entity_def;

            load_entity(archive);
        }
    }
    else
    {
        Logger::warning("Loading scene with no entities");
    }
}

void SceneLoader::load_entity(const Archive& archive)
{
    // TODO: instead of crashing if no type is provided we should error out
    const std::string entity_type = archive.read<std::string>("type");
    const std::string entity_name = archive.read_or<std::string>("name");

    if (const auto reflected_type = ReflectionDatabase::get().reflect_type(entity_type))
    {
        const peng::weak_ptr<Entity> entity = EntityFactory::get().create_entity(reflected_type.to_shared_ref(), entity_name);

        entity->deserialize(archive);
        load_components(archive.json_def, entity);
    }
    else
    {
        Logger::error(
            "Could not load entity '%s' as the type '%s' does not exist",
            entity_name.c_str(), entity_type.c_str()
        );
    }
}

void SceneLoader::load_components(const nlohmann::json& entity_def, const peng::weak_ptr<Entity>& entity)
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
            Archive archive;
            archive.json_def = component_def;

            load_component(archive, entity);
        }
    }
}

void SceneLoader::load_component(const Archive& archive, const peng::weak_ptr<Entity>& entity)
{
    const bool inline_def = archive.json_def.is_string();

    if (!(inline_def || archive.json_def.is_object()))
    {
        Logger::error(
            "Could not load component '%s' on entity '%s' as it is not a component typename or definition",
            archive.json_def.dump().c_str(), entity->name().c_str()
        );
        return;
    }

    const std::string component_type =
        inline_def
        ? archive.json_def.get<std::string>()
        : archive.read<std::string>("type");

    if (const auto reflected_type = ReflectionDatabase::get().reflect_type(component_type))
    {
        const peng::weak_ptr<Component> component = ComponentFactory::get().create_component(reflected_type.to_shared_ref(), entity);
        if (!inline_def)
        {
            component->deserialize(archive);
        }
    }
    else
    {
        Logger::error(
            "Could not add component '%s' to entity '%s' as the type does not exist",
            component_type.c_str(), entity->name().c_str()
        );
    }
}
