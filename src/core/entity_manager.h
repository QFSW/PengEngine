#pragma once

#include "entity.h"

#include <vector>
#include <concepts>

#include <memory/shared_ref.h>
#include <memory/weak_ptr.h>

enum class EntityState
{
	invalid,
	valid,
	pending_add,
	pending_kill
};

class EntityManager
{
public:
	EntityManager();

	// ----------- Engine API -----------
	void tick(float delta_time);
	void shutdown();
	// ----------------------------------

	// ------------ User API ------------
	template <std::derived_from<Entity> T, typename...Args>
	requires std::constructible_from<T, Args...>
	peng::weak_ptr<T> create_entity(Args&&...args);

	void destroy_entity(const peng::weak_ptr<Entity>& entity);

	[[nodiscard]] EntityState get_entity_state(const peng::weak_ptr<Entity>& entity) const;
	[[nodiscard]] peng::weak_ptr<Entity> find_entity(const std::string& entity_name, bool include_inactive) const;

	void dump_hierarchy() const;
	// ----------------------------------

private:
	void tick_entities(float delta_time);
	void flush_pending_adds();
	void flush_pending_kills();

	[[nodiscard]] std::string build_entity_hierarchy(const std::vector<peng::weak_ptr<Entity>>& root_entities) const;

	void build_entity_hierarchy(
		const std::vector<peng::weak_ptr<Entity>>& root_entities,
		int32_t depth,
		std::vector<bool>& draw_vertical,
		std::string& result
	) const;

	std::vector<TickGroup> _tick_groups;
	std::vector<peng::shared_ref<Entity>> _entities;
	std::vector<peng::shared_ref<Entity>> _pending_adds;
	std::vector<peng::weak_ptr<Entity>> _pending_kills;
};

template <std::derived_from<Entity> T, typename...Args>
requires std::constructible_from<T, Args...>
peng::weak_ptr<T> EntityManager::create_entity(Args&&...args)
{
	peng::shared_ref<T> entity = peng::make_shared<T>(std::forward<Args>(args)...);
	_pending_adds.push_back(entity);

	return entity;
}