#pragma once

#include <concepts>
#include <memory>
#include <unordered_map>

#include <utils/check.h>

// ReSharper disable once CppUnusedIncludeDirective
#include "subsystem_definition.h"

class Subsystem
{
public:
    template <std::derived_from<Subsystem> T>
    static void load();

    static void start_all();
    static void shutdown_all();
    static void tick_all(float delta_time);

    Subsystem() = default;
    Subsystem(const Subsystem&) = delete;
    Subsystem(Subsystem&&) = delete;
    Subsystem& operator=(const Subsystem&) = delete;
    Subsystem& operator=(Subsystem&&) = delete;
    virtual ~Subsystem() = default;

    virtual void start() = 0;
    virtual void shutdown() = 0;
    virtual void tick(float delta_time) = 0;

protected:
    template <std::derived_from<Subsystem> T>
    static T& get();

private:
    static std::vector<std::unique_ptr<Subsystem>> _subsystems;
    static std::unordered_map<const type_info*, size_t> _subsystem_map;
};

template <std::derived_from<Subsystem> T>
void Subsystem::load()
{
    const type_info* id = &typeid(T);
    check(!_subsystem_map.contains(id));

    _subsystem_map[id] = _subsystems.size();
    _subsystems.push_back(std::make_unique<T>());
}

template <std::derived_from<Subsystem> T>
T& Subsystem::get()
{
    const type_info* id = &typeid(T);
    check(_subsystem_map.contains(id));

    return static_cast<T&>(*_subsystems[_subsystem_map[id]]);
}
