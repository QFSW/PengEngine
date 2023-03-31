#pragma once

#include <string>

#include <libs/nlohmann/json.hpp>

struct AssetDefinition
{
    std::string path;
    nlohmann::json json_def;
};