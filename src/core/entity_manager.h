#pragma once

#include "entity.h"

#include <memory>
#include <vector>
#include <concepts>
#include <cassert>

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
	std::weak_ptr<T> create_entity(Args&&...args);

	template <typename T>
	requires std::derived_from<T, Entity>
	void destroy_entity(const std::weak_ptr<T>& entity);

	template <typename T>
	requires std::derived_from<T, Entity>
	EntityState get_entity_state(const std::weak_ptr<T>& entity) const;
	// ----------------------------------

private:
	void tick_entities(double delta_time);
	void flush_pending_adds();
	void flush_pending_kills();

	std::vector<std::shared_ptr<Entity>> _entities;
	std::vector<std::shared_ptr<Entity>> _pending_adds;
	std::vector<std::weak_ptr<Entity>> _pending_kills;
};

template <typename T, typename...Args>
requires std::derived_from<T, Entity>
std::weak_ptr<T> EntityManager::create_entity(Args&&...args)
{
	std::shared_ptr<T> entity = std::make_shared<T>(std::forward<Args>(args)...);
	_pending_adds.push_back(std::static_pointer_cast<Entity>(entity));

	return entity;
}

template <typename T>
requires std::derived_from<T, Entity>
void EntityManager::destroy_entity(const std::weak_ptr<T>& entity)
{
	assert(!entity.expired());

	_pending_kills.push_back(entity);
}

template <typename T>
requires std::derived_from<T, Entity>
EntityState EntityManager::get_entity_state(const std::weak_ptr<T>& entity) const
{
	const std::shared_ptr<Entity> strong_entity = std::static_pointer_cast<Entity>(entity.lock());

	if (vectools::contains(_pending_adds, strong_entity))
	{
		return EntityState::PendingAdd;
	}

	if (vectools::contains(_entities, strong_entity))
	{
		return EntityState::Valid;
	}

	const std::weak_ptr<Entity> weak_entity = strong_entity;
	if (vectools::contains(_pending_kills, weak_entity))
	{
		return EntityState::PendingKill;
	}

	return EntityState::Invalid;
}