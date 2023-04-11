#pragma once

#include <string>

#include <libs/nlohmann/json.hpp>

// Data archive used for serializing/deserializing items
struct Archive
{
    // The path to the archive on disk, if any
    std::string path;

    // The json object for this archive
    nlohmann::json json_def;

    template <typename T>
    T read(const char* name) const;

    template <typename T>
    void write(const char* name, const T& in);

    template <typename T>
    T read_or(const char* name, const T& default_val = T()) const;

    template <typename T>
    void try_read(const char* name, T& out) const;
};

template <typename T>
T Archive::read(const char* name) const
{
    return json_def[name].get<T>();
}

template <typename T>
void Archive::write(const char* name, const T& in)
{
    json_def[name] = in;
}

template <typename T>
T Archive::read_or(const char* name, const T& default_val) const
{
    return json_def.value(name, default_val);
}

template <typename T>
void Archive::try_read(const char* name, T& out) const
{
    out = json_def.value(name, out);
}
