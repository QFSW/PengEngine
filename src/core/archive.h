#pragma once

#include <string>

#include <libs/nlohmann/json.hpp>
#include <math/json_support.h>

// Data archive used for serializing/deserializing items
struct Archive
{
    // Creates an archive from a file on disk
    static Archive from_disk(const std::string& path);

    // The name of the item represented by this archive
    std::string name;

    // The path to the archive on disk, if any
    std::string path;

    // The json object for this archive
    nlohmann::json json_def;

    template <typename T>
    T read(const char* key) const;

    template <typename T>
    void write(const char* key, const T& in);

    template <typename T>
    T read_or(const char* key, const T& default_val = T()) const;

    template <typename T>
    void try_read(const char* key, T& out) const;
};

template <typename T>
T Archive::read(const char* key) const
{
    return json_def[key].get<T>();
}

template <typename T>
void Archive::write(const char* key, const T& in)
{
    json_def[key] = in;
}

template <typename T>
T Archive::read_or(const char* key, const T& default_val) const
{
    if (json_def.is_object())
    {
        return json_def.value(key, default_val);
    }

    return default_val;
}

template <typename T>
void Archive::try_read(const char* key, T& out) const
{
    if (const auto it = json_def.find(key); it != json_def.end())
    {
        it->get_to(out);
    }
}
