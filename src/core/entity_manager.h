#pragma once

#include "entity.h"

#include <memory>
#include <vector>
#include <concepts>
#include <cassert>

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
	// ----------------------------------

private:
	void tick_entities(double delta_time);
	void cleanup_entities();

	std::vector<std::shared_ptr<Entity>> _entities;
	std::vector<std::weak_ptr<Entity>> _pending_kills;
};

template <typename T, typename...Args>
requires std::derived_from<T, Entity>
std::weak_ptr<T> EntityManager::create_entity(Args&&...args)
{
	std::shared_ptr<T> entity = std::make_shared<T>(std::forward<Args>(args)...);
	_entities.push_back(std::static_pointer_cast<Entity>(entity));

	return entity;
}

template <typename T>
requires std::derived_from<T, Entity>
void EntityManager::destroy_entity(const std::weak_ptr<T>& entity)
{
	assert(!entity.expired());

	_pending_kills.push_back(entity);
}