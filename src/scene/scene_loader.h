#pragma once

#include <libs/nlohmann/json.hpp>

namespace scene
{
    class SceneLoader
    {
    public:
        void load_from_file(const std::string& path);
        void load_from_json(const nlohmann::json& world_def);

    private:
        void load_entities(const nlohmann::json& world_def);
    };
}
