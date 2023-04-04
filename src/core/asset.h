#pragma once

#include <fstream>

#include <core/logger.h>
#include <memory/shared_ptr.h>
#include <memory/weak_ptr.h>
#include <profiling/scoped_event.h>

#include "asset_definition.h"

// Concept for an item that can be contained as an asset
template <typename T>
concept CAsset = requires(const AssetDefinition& asset_def)
{
    { T::load_asset(asset_def) } -> std::convertible_to<peng::shared_ref<T>>;
};

// Base interface for all Assets
class IAsset
{
public:
    [[nodiscard]] virtual bool loaded() const noexcept = 0;
};

// Generalized resource container for an asset T that meets the CAsset criteria
// If two assets are loaded with the same path they will resolve to the same item in memory
template <CAsset T>
class Asset : public IAsset
{
public:
    explicit Asset(const std::string& path);
    explicit Asset(std::string&& path);

    [[nodiscard]] peng::shared_ref<T> load_mutable();
    [[nodiscard]] peng::shared_ref<const T> load();
    [[nodiscard]] bool loaded() const noexcept override;

private:
    std::string _path;

    static std::unordered_map<std::string, peng::weak_ptr<T>> _asset_map;
};

template <CAsset T>
std::unordered_map<std::string, peng::weak_ptr<T>> Asset<T>::_asset_map;

template <CAsset T>
Asset<T>::Asset(const std::string& path)
    : _path(path)
{ }

template <CAsset T>
Asset<T>::Asset(std::string&& path)
    : _path(path)
{ }

template <CAsset T>
peng::shared_ref<T> Asset<T>::load_mutable()
{
    peng::weak_ptr<T>& existing = _asset_map[_path];

    if (peng::shared_ptr<T> existing_locked = existing.lock())
    {
        return existing_locked.to_shared_ref();
    }
    else
    {
        SCOPED_EVENT("Loading asset", _path.c_str());
        Logger::log("Loading asset '%s'", _path.c_str());

        // TODO: add better error handling
        std::ifstream file(_path);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open asset " + _path);
        }

        AssetDefinition asset_def;
        asset_def.path = _path;
        asset_def.json_def = nlohmann::json::parse(file);

        peng::shared_ref<T> loaded = T::load_asset(asset_def);
        existing = loaded;

        return loaded;
    }
}

template <CAsset T>
peng::shared_ref<const T> Asset<T>::load()
{
    return load_mutable();
}

template <CAsset T>
bool Asset<T>::loaded() const noexcept
{
    return _asset_map[_path].valid();
}
