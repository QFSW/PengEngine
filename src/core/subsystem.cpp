#include "subsystem.h"

#include <ranges>

#include <profiling/scoped_event.h>

std::vector<std::unique_ptr<Subsystem>> Subsystem::_subsystems;
std::unordered_map<const std::type_info*, size_t> Subsystem::_subsystem_map;

void Subsystem::start_all()
{
    SCOPED_EVENT("Starting subsystems");
    for (const auto& subsystem : _subsystems)
    {
        subsystem->start();
    }
}

void Subsystem::shutdown_all()
{
    SCOPED_EVENT("Shutting down subsystems");
    for (const auto& subsystem : _subsystems | std::views::reverse)
    {
        subsystem->shutdown();
    }
}

void Subsystem::tick_all(float delta_time)
{
    SCOPED_EVENT("Ticking subsystems");
    for (const auto& subsystem : _subsystems)
    {
        subsystem->tick(delta_time);
    }
}
