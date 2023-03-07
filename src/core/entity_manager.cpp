#include "entity_manager.h"

#include <cassert>
#include <utils/vectools.h>
#include <profiling/scoped_event.h>

#include "entity.h"
#include "component.h"
#include "logger.h"

EntityManager::EntityManager()
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

void EntityManager::tick(float delta_time)
{
	SCOPED_EVENT("EntityManager - tick");

	flush_pending_adds();
	tick_entities(delta_time);
	flush_pending_kills();
}

void EntityManager::shutdown()
{
	SCOPED_EVENT("EntityManager - shutdown");
	Logger::log("Destroying all entities");

	for (peng::shared_ref<Entity>& entity : _entities)
	{
		// TODO: add a destroy reason (explicit / shutdown)
		entity->pre_destroy();
	}

	_pending_adds.clear();
	_entities.clear();
}

void EntityManager::register_entity(const peng::shared_ref<Entity>& entity)
{
	_pending_adds.push_back(entity);
}

void EntityManager::destroy_entity(const peng::weak_ptr<Entity>& entity)
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

EntityState EntityManager::get_entity_state(const peng::weak_ptr<Entity>& entity) const
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

peng::weak_ptr<Entity> EntityManager::find_entity(const std::string& entity_name, bool include_inactive) const
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

void EntityManager::dump_hierarchy() const
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

void EntityManager::tick_entities(float delta_time)
{
	for (size_t i = 0; i < _tick_groups.size(); i++)
	{
		SCOPED_EVENT("EntityManager - ticking entity group", _tick_group_names[i].c_str());
		const TickGroup tick_group = _tick_groups[i];

		for (const peng::shared_ref<Entity>& entity : _entities)
		{
			if (entity->active_in_hierarchy())
			{
				if (entity->tick_group() == tick_group)
				{
					entity->tick(delta_time);
				}

				for (const peng::shared_ref<Component>& component : entity->components())
				{
					if (component->tick_group() == tick_group)
					{
						component->tick(delta_time);
					}
				}
			}
		}
	}
}

void EntityManager::flush_pending_adds()
{
	SCOPED_EVENT("EntityManager - flush pending adds");

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

void EntityManager::flush_pending_kills()
{
	SCOPED_EVENT("EntityManager - flush pending kills");

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

std::string EntityManager::build_entity_hierarchy(const std::vector<peng::weak_ptr<Entity>>& root_entities) const
{
	std::string result;
	std::vector<bool> draw_vertical;

	build_entity_hierarchy(root_entities, 0, draw_vertical, result);
	return result;
}

void EntityManager::build_entity_hierarchy(
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
