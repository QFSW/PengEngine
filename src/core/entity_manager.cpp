#include "entity_manager.h"

#include <utils/vectools.h>

void EntityManager::tick(double delta_time)
{
	flush_pending_adds();
	tick_entities(delta_time);
	flush_pending_kills();
}

void EntityManager::tick_entities(double delta_time)
{
	for (std::shared_ptr<Entity>& entity : _entities)
	{
		if (entity->can_tick())
		{
			entity->tick(delta_time);
		}
	}
}

void EntityManager::flush_pending_adds()
{
	for (std::shared_ptr<Entity>& entity : _pending_adds)
	{
		_entities.push_back(entity);
	}

	_pending_adds.clear();
}

void EntityManager::flush_pending_kills()
{
	vectools::remove_all<std::shared_ptr<Entity>>(_entities, [this](const std::shared_ptr<Entity>& entity) {
		for (const std::weak_ptr<Entity>& pending_kill : _pending_kills)
		{
			if (pending_kill.lock() == entity)
			{
				assert(entity.use_count() == 1);
				return true;
			}
		}

		return false;
	});

	_pending_kills.clear();
}