#include "entity_subsystem.h"

#include <algorithm>
#include <utils/vectools.h>
#include <utils/execution.h>
#include <profiling/scoped_event.h>

#include "entity.h"
#include "component.h"
#include "logger.h"

EntitySubsystem::EntitySubsystem()
    : Subsystem()
{
	constexpr int32_t start = static_cast<int32_t>(TickGroup::standard);
	constexpr int32_t end = static_cast<int32_t>(TickGroup::none);

	for (int32_t tick_group = start; tick_group < end; tick_group++)
	{
		const TickGroup group = static_cast<TickGroup>(tick_group);
		_tick_groups.push_back(group);
		_tick_group_names.push_back(strtools::cat(group));
	}
}

void EntitySubsystem::start()
{
    
}

void EntitySubsystem::shutdown()
{
	SCOPED_EVENT("EntitySubsystem - shutdown");
	Logger::log("Destroying all entities");

	for (peng::shared_ref<Entity>& entity : _entities)
	{
		// TODO: add a destroy reason (explicit / shutdown)
		entity->pre_destroy();
	}

	_pending_adds.clear();
	_entities.clear();
}

void EntitySubsystem::tick(float delta_time)
{
	SCOPED_EVENT("EntitySubsystem - tick");

	flush_pending_actions();
	tick_entities(delta_time);
}

void EntitySubsystem::register_entity(const peng::shared_ref<Entity>& entity)
{
	entity->_constructed = true;
	_pending_adds.push_back(entity);
}

void EntitySubsystem::destroy_entity(const peng::weak_ptr<Entity>& entity)
{
	if (!entity.valid())
	{
		Logger::error("Cannot destroy invalid entity");
		return;
	}

	// TODO: check if entity is already queued for destruction
	_pending_kills.push_back(entity);
	for (const peng::weak_ptr<Entity>& child : entity->children())
	{
		destroy_entity(child);
	}
}

EntityState EntitySubsystem::get_entity_state(const peng::weak_ptr<Entity>& entity) const
{
	if (vectools::contains(_pending_kills, entity))
	{
		return EntityState::pending_kill;
	}

	if (const peng::shared_ptr<Entity> strong_entity = entity.lock())
	{
		const peng::shared_ref<Entity> entity_ref = strong_entity.to_shared_ref();
		if (vectools::contains(_entities, entity_ref))
		{
			return EntityState::valid;
		}
		if (vectools::contains(_pending_adds, entity_ref))
		{
			return EntityState::pending_add;
		}
	}

	return EntityState::invalid;
}

peng::weak_ptr<Entity> EntitySubsystem::find_entity(const std::string& entity_name, bool include_inactive) const
{
	for (const peng::shared_ref<Entity>& entity : _entities)
	{
		if (include_inactive || entity->active_in_hierarchy())
		{
			if (entity->name() == entity_name)
			{
				return entity;
			}
		}
	}

	return {};
}

std::vector<peng::weak_ptr<Entity>> EntitySubsystem::all_entities()
{
	std::vector<peng::weak_ptr<Entity>> result;
	result.reserve(_entities.size());

	for (const auto& x : _entities)
	{
		result.emplace_back(x);
	}

	return result;
}

void EntitySubsystem::dump_hierarchy() const
{
	if constexpr (!Logger::enabled())
	{
		return;
	}

	std::vector<peng::weak_ptr<Entity>> root_entities;
	for (const peng::shared_ref<Entity>& entity : _entities)
	{
		if (!entity->parent().valid())
		{
			root_entities.emplace_back(entity);
		}
	}

	const std::string hierarchy = build_entity_hierarchy(root_entities);
	if (hierarchy.empty())
	{
		Logger::log("No entities exist");
	}
	else
	{
		Logger::log("Entity hierarchy\n%s", hierarchy.c_str());
	}
}

void EntitySubsystem::tick_entities(float delta_time)
{
	std::vector<peng::shared_ref<ITickable>> tickables;

	for (size_t i = 0; i < _tick_groups.size(); i++)
	{
		const TickGroup tick_group = _tick_groups[i];

		{
			SCOPED_EVENT("EntitySubsystem - pre tick entity group", _tick_group_names[i].c_str());
			_pre_tick_entity_group.invoke(tick_group);
		}

		// Gather tickables for the current group
		for (const peng::shared_ref<Entity>& entity : _entities)
		{
			if (entity->active_in_hierarchy())
			{
				if (entity->tick_group() == tick_group)
				{
					tickables.emplace_back(entity);
				}

				for (const peng::shared_ref<Component>& component : entity->components())
				{
					if (component->tick_group() == tick_group)
					{
						tickables.emplace_back(component);
					}
				}
			}
		}

		{
			SCOPED_EVENT("EntitySubsystem - ticking entity group", _tick_group_names[i].c_str());

			for_each_tickable(
				is_parallel_tick_group(tick_group),
				tickables,
				[&](const peng::shared_ref<ITickable>& tickable)
				{
					tickable->tick(delta_time);
				});
		}

		// Flush pending lifecycle updates (creation/destruction) after each group
		tickables.clear();
		flush_pending_actions();

		{
			SCOPED_EVENT("EntitySubsystem - post tick entity group", _tick_group_names[i].c_str());
			_post_tick_entity_group.invoke(tick_group);
		}
	}
}

void EntitySubsystem::flush_pending_actions()
{
	flush_pending_kills();
	flush_pending_adds();
}

template <typename F>
void EntitySubsystem::for_each_tickable(bool parallel, const std::vector<peng::shared_ref<ITickable>>& tickables, F&& invocable)
{
	if (parallel)
	{
		std::for_each(EXEC_PAR tickables.begin(), tickables.end(), std::move(invocable));
	}
	else
	{
		std::for_each(EXEC_SEQ tickables.begin(), tickables.end(), std::move(invocable));
	}
}

void EntitySubsystem::flush_pending_adds()
{
	const std::vector staged_adds(std::move(_pending_adds));

	for (const peng::shared_ref<Entity>& entity : staged_adds)
	{
		_entities.push_back(entity);
	}

	for (const peng::shared_ref<Entity>& entity : staged_adds)
	{
		entity->post_create();
	}
}

void EntitySubsystem::flush_pending_kills()
{
	auto kill_in_buffer = [&](std::vector<peng::shared_ref<Entity>>& entities, bool exists_yet)
	{
		for (int32_t entity_index = static_cast<int32_t>(entities.size() - 1); entity_index >= 0; entity_index--)
		{
			peng::shared_ref<Entity>& entity = entities[entity_index];
			peng::weak_ptr<Entity> weak_entity = entity;

			if (vectools::contains(_pending_kills, weak_entity))
			{
				if (exists_yet)
				{
					entity->pre_destroy();
				}

				entities.erase(entities.begin() + entity_index);

				if (weak_entity.valid())
				{
					Logger::warning(
						"Entity '%s' still exists after kill, potential leak",
						weak_entity->name().c_str()
					);
				}
			}
		}
	};

	kill_in_buffer(_entities, true);
	kill_in_buffer(_pending_adds, false);

	_pending_kills.clear();
}

std::string EntitySubsystem::build_entity_hierarchy(const std::vector<peng::weak_ptr<Entity>>& root_entities) const
{
	std::string result;
	std::vector<bool> draw_vertical;

	build_entity_hierarchy(root_entities, 0, draw_vertical, result);
	return result;
}

void EntitySubsystem::build_entity_hierarchy(
	const std::vector<peng::weak_ptr<Entity>>& root_entities,
	int32_t depth,
	std::vector<bool>& draw_vertical,
	std::string& result
) const
{
	constexpr int32_t indentation = 3;
	constexpr char vertical_symbol = '|';
	constexpr char horizontal_symbol = '-';
	constexpr char vertical_split_symbol = '|';
	constexpr char terminal_symbol = '|';

	for (size_t root_index = 0; root_index < root_entities.size(); root_index++)
	{
		const peng::weak_ptr<Entity>& root = root_entities[root_index];

		for (int32_t d = 0; d < depth; d++)
		{
			result += draw_vertical[d] ? vertical_symbol : ' ';
			result += std::string(indentation - 1, ' ');
		}

		const bool terminal = root_index == root_entities.size() - 1;
		draw_vertical.push_back(!terminal);

		result += terminal ? terminal_symbol : vertical_split_symbol;
		result += std::string(indentation - 1, horizontal_symbol);

		if (root.valid())
		{
			result += root->name() + "\n";
			build_entity_hierarchy(root->children(), depth + 1, draw_vertical, result);
		}
		else
		{
			result += "??? Invalid Entity ???\n";
		}

		draw_vertical.pop_back();
	}
}
