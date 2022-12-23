#include "entity_manager.h"

#include <utils/vectools.h>

void EntityManager::tick(double delta_time)
{
	flush_pending_adds();
	tick_entities(delta_time);
	flush_pending_kills();
}

void EntityManager::destroy_entity(const peng::weak_ptr<Entity>& entity)
{
	assert(entity.valid());

	_pending_kills.push_back(entity);
}

EntityState EntityManager::get_entity_state(const peng::weak_ptr<Entity>& entity) const
{
	if (vectools::contains(_pending_kills, entity))
	{
		return EntityState::PendingKill;
	}

	const peng::shared_ptr<Entity> strong_entity = entity.lock();
	if (strong_entity)
	{
		const peng::shared_ref<Entity> entity_ref = strong_entity.to_shared_ref();
		if (vectools::contains(_entities, entity_ref))
		{
			return EntityState::Valid;
		}
		if (vectools::contains(_pending_adds, entity_ref))
		{
			return EntityState::PendingAdd;
		}
	}

	return EntityState::Invalid;
}

void EntityManager::tick_entities(double delta_time)
{
	for (peng::shared_ref<Entity>& entity : _entities)
	{
		if (entity->can_tick())
		{
			entity->tick(delta_time);
		}
	}
}

void EntityManager::flush_pending_adds()
{
	for (peng::shared_ref<Entity>& entity : _pending_adds)
	{
		_entities.push_back(entity);
	}

	_pending_adds.clear();
}

void EntityManager::flush_pending_kills()
{
	vectools::remove_all<peng::shared_ref<Entity>>(_entities, [this](const peng::shared_ref<Entity>& entity) {
		for (const peng::weak_ptr<Entity>& pending_kill : _pending_kills)
		{
			if (pending_kill == entity)
			{
				assert(entity.use_count() == 1);
				return true;
			}
		}

		return false;
	});

	_pending_kills.clear();
}