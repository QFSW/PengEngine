#include "entity_manager.h"

#include <utils/vectools.h>

void EntityManager::tick(double delta_time)
{
	tick_entities(delta_time);
	cleanup_entities();
}

void EntityManager::tick_entities(double delta_time)
{
	for (std::shared_ptr<Entity>& entity : _entities)
	{
		entity->tick(delta_time);
	}
}

void EntityManager::cleanup_entities()
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