#pragma once

#include <fstream>
#include <filesystem>

#include <core/logger.h>
#include <memory/shared_ptr.h>
#include <memory/weak_ptr.h>
#include <profiling/scoped_event.h>

#include "archive.h"

// Concept for an item that can be contained as an asset
template <typename T>
concept CAsset = requires(const Archive& archive)
{
    { T::load_asset(archive) } -> std::convertible_to<peng::shared_ref<T>>;
};

// Base interface for all Assets
class IAsset
{
public:
    [[nodiscard]] virtual bool loaded() const noexcept = 0;
    [[nodiscard]] virtual bool exists() const noexcept = 0;
    [[nodiscard]] virtual bool empty() const noexcept = 0;
    [[nodiscard]] virtual const std::string& path() const noexcept = 0;
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
    [[nodiscard]] bool exists() const noexcept override;
    [[nodiscard]] bool empty() const noexcept override;
    [[nodiscard]] const std::string& path() const noexcept override;

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
        check(exists());

        Archive archive;
        archive.path = _path;
        archive.json_def = nlohmann::json::parse(file);

        {
            namespace fs = std::filesystem;
            archive.name = archive.read_or<std::string>(
                "name", fs::path(_path).filename().string()
            );
        }

        peng::shared_ref<T> loaded = T::load_asset(archive);
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

template <CAsset T>
bool Asset<T>::exists() const noexcept
{
    return std::filesystem::exists(_path);
}

template <CAsset T>
bool Asset<T>::empty() const noexcept
{
    return _path.empty();
}

template <CAsset T>
const std::string& Asset<T>::path() const noexcept
{
    return _path;
}

// JSON support for assets
#pragma region JSON

template <CAsset T>
void to_json(nlohmann::json& j, const Asset<T>& in)
{
    j = in.path();
}

template <CAsset T>
void from_json(const nlohmann::json& j, Asset<T>& out)
{
    out = Asset<T>(j.get<std::string>());
}

namespace peng
{
    template <CAsset T>
    void to_json(nlohmann::json&, const shared_ref<const T>&)
    {
        // TODO: implement serialization
        check(false);
    }

    template <CAsset T>
    void from_json(const nlohmann::json& j, shared_ref<const T>& out)
    {
        check(j.is_string());
        Asset<T> asset(j.get<std::string>());
        out = asset.load();
    }

    template <CAsset T>
    void from_json(const nlohmann::json& j, shared_ref<T>& out)
    {
        check(j.is_string());
        Asset<T> asset(j.get<std::string>());
        out = asset.load_mutable();
    }

    template <CAsset T>
    void to_json(nlohmann::json&, const shared_ptr<const T>&)
    {
        // TODO: implement serialization
        check(false);
    }

    template <CAsset T>
    void from_json(const nlohmann::json& j, shared_ptr<const T>& out)
    {
        if (j.is_null())
        {
            out = {};
        }

        check(j.is_string());
        Asset<T> asset(j.get<std::string>());
        out = asset.load();
    }

    template <CAsset T>
    void from_json(const nlohmann::json& j, shared_ptr<T>& out)
    {
        if (j.is_null())
        {
            out = {};
        }

        check(j.is_string());
        Asset<T> asset(j.get<std::string>());
        out = asset.load_mutable();
    }
}

#pragma endregion