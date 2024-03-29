#pragma once

#include <memory/weak_ptr.h>

#include "logger.h"
#include "item_factory.h"
#include "reflection_database.h"
#include "entity_subsystem.h"

struct Archive;
class Entity;

class EntityFactory : public utils::Singleton<EntityFactory>
{
	using Singleton::Singleton;

public:
	template <std::derived_from<Entity> T>
	void register_entity();

	// Creates an entity with the given type and name
	// Returns nullptr if the entity could not be created because
	// it has no viable constructors
	peng::weak_ptr<Entity> create_entity(// NOLINT(modernize-use-nodiscard)
		const peng::shared_ref<const ReflectedType>& entity_type,
		const std::string& entity_name = ""
	) const;

	// Loads an entity from the provided archive
	// Creates a new entity with the correct components and serialized members
	// Returns nullptr if the load failed
	peng::weak_ptr<Entity> load_entity(const Archive& archive);

private:
	struct EntityConstructorSet
	{
		bool has_default = false;
		bool has_named = false;
	};

	// Loads components from the entity archive onto the entity
	void load_components(const Archive& entity_archive, const peng::weak_ptr<Entity>& entity);

	// Loads child entities onto the entity
	void load_entity_children(const Archive& entity_archive, const peng::weak_ptr<Entity>& entity);

	ItemFactory<peng::weak_ptr<Entity>> _default_factory;
	ItemFactory<peng::weak_ptr<Entity>, const std::string&> _named_factory;
	std::unordered_map<peng::shared_ref<const ReflectedType>, EntityConstructorSet> _type_to_constructor_set;
};

template <std::derived_from<Entity> T>
void EntityFactory::register_entity()
{
	const peng::shared_ref<const ReflectedType> entity_type = ReflectionDatabase::get().reflect_type_checked<T>();
	EntityConstructorSet constructor_set;

	if constexpr (std::is_constructible_v<T>)
	{
		const auto entity_constructor = []() -> peng::weak_ptr<Entity>
		{
			return EntitySubsystem::get().create_entity<T>();
		};

		_default_factory.register_factory(entity_type, entity_constructor);
		constructor_set.has_default = true;
	}

	if constexpr (std::is_constructible_v<T, const std::string&>)
	{
		const auto entity_constructor = [](const std::string& entity_name) -> peng::weak_ptr<Entity>
		{
			return EntitySubsystem::get().create_entity<T>(entity_name);
		};

		_named_factory.register_factory(entity_type, entity_constructor);
		constructor_set.has_named = true;
	}

	if (constructor_set.has_default || constructor_set.has_named)
	{
		_type_to_constructor_set[entity_type] = constructor_set;
	}
	else
	{
		Logger::warning(
			"Entity type '%s' cannot be registered to the entity factory due to missing all viable constructors",
			entity_type->name.c_str()
		);
	}
}
