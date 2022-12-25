#pragma once

#include "entity.h"

#include <vector>
#include <concepts>

#include <memory/shared_ref.h>
#include <memory/weak_ptr.h>
#include <utils/vectools.h>

class EntityManager
{
public:
	// ----------- Engine API -----------
	void tick(double delta_time);
	// ----------------------------------

	// ------------ User API ------------
	template <typename T, typename...Args>
	requires std::derived_from<T, Entity>
	peng::weak_ptr<T> create_entity(Args&&...args);

	void destroy_entity(const peng::weak_ptr<Entity>& entity);
	EntityState get_entity_state(const peng::weak_ptr<Entity>& entity) const;
	// ----------------------------------

private:
	void tick_entities(double delta_time);
	void flush_pending_adds();
	void flush_pending_kills();

	std::vector<peng::shared_ref<Entity>> _entities;
	std::vector<peng::shared_ref<Entity>> _pending_adds;
	std::vector<peng::weak_ptr<Entity>> _pending_kills;
};

template <typename T, typename...Args>
requires std::derived_from<T, Entity>
peng::weak_ptr<T> EntityManager::create_entity(Args&&...args)
{
	peng::shared_ref<T> entity = peng::make_shared<T>(std::forward<Args>(args)...);
	_pending_adds.push_back(entity);

	return entity;
}