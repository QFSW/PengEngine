#pragma once

#include <vector>
#include <concepts>

#include <memory/shared_ref.h>
#include <memory/weak_ptr.h>
#include <utils/event.h>

#include "tickable.h"

enum class EntityState
{
	invalid,
	valid,
	pending_add,
	pending_kill
};

class Entity;

class EntityManager
{
	DEFINE_EVENT(pre_tick_entity_group, TickGroup)
	DEFINE_EVENT(post_tick_entity_group, TickGroup)

public:
	EntityManager();

	// ----------- Engine API -----------
	void tick(float delta_time);
	void shutdown();
	// ----------------------------------

	// ------------ User API ------------

	// Construct and register an entity with the entity manager
	template <std::derived_from<Entity> T, typename...Args>
	requires std::constructible_from<T, Args...>
	peng::weak_ptr<T> create_entity(Args&&...args);

	// Register an entity that was constructed externally with the entity manager
	// Once registered, the entity manager is responsible for the lifetime of the entity
	void register_entity(const peng::shared_ref<Entity>& entity);

	// Destroys an entity owned by the entity manager
	void destroy_entity(const peng::weak_ptr<Entity>& entity);

	[[nodiscard]] EntityState get_entity_state(const peng::weak_ptr<Entity>& entity) const;
	[[nodiscard]] peng::weak_ptr<Entity> find_entity(const std::string& entity_name, bool include_inactive) const;
	// TODO: add templated and non templated functions for finding entity/entities via type

	void dump_hierarchy() const;
	// ----------------------------------

private:
	void tick_entities(float delta_time);
	void flush_pending_adds();
	void flush_pending_kills();

	[[nodiscard]] std::string build_entity_hierarchy(const std::vector<peng::weak_ptr<Entity>>& root_entities) const;

	template <typename F>
	void for_each_entity(bool parallel, F&& invocable);

	void build_entity_hierarchy(
		const std::vector<peng::weak_ptr<Entity>>& root_entities,
		int32_t depth,
		std::vector<bool>& draw_vertical,
		std::string& result
	) const;

	std::vector<TickGroup> _tick_groups;
	std::vector<std::string> _tick_group_names;
	std::vector<peng::shared_ref<Entity>> _entities;
	std::vector<peng::shared_ref<Entity>> _pending_adds;
	std::vector<peng::weak_ptr<Entity>> _pending_kills;
};

template <std::derived_from<Entity> T, typename...Args>
requires std::constructible_from<T, Args...>
peng::weak_ptr<T> EntityManager::create_entity(Args&&...args)
{
	peng::shared_ref<T> entity = peng::make_shared<T>(std::forward<Args>(args)...);
	register_entity(entity);

	return entity;
}
