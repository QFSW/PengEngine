#include "scene_loader.h"

#include <fstream>

#include <core/archive.h>
#include <core/entity_factory.h>
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

            EntityFactory::get().load_entity(archive);
        }
    }
    else
    {
        Logger::warning("Loading scene with no entities");
    }
}
