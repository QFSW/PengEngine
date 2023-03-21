#include "subsystem.h"

#include <ranges>

std::vector<std::unique_ptr<Subsystem>> Subsystem::_subsystems;
std::unordered_map<const type_info*, size_t> Subsystem::_subsystem_map;

void Subsystem::start_all()
{
    for (const auto& subsystem : _subsystems)
    {
        subsystem->start();
    }
}

void Subsystem::shutdown_all()
{
    for (const auto& subsystem : _subsystems | std::views::reverse)
    {
        subsystem->shutdown();
    }
}