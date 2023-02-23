#pragma once

#include <memory/weak_ptr.h>

#include "logger.h"
#include "item_factory.h"
#include "reflection_database.h"
#include "peng_engine.h"

class Entity;

class EntityFactory
{
public:
	static EntityFactory& get();

	EntityFactory(const EntityFactory&) = delete;
	EntityFactory(EntityFactory&&) = delete;

	template <typename T>
	void register_entity();

	// NOLINT(modernize-use-nodiscard)
	peng::weak_ptr<Entity> create_entity(
		peng::shared_ref<const ReflectedType> entity_type,
		const std::string& entity_name = ""
	) const;

private:
	EntityFactory() = default;

	struct EntityConstructorSet
	{
		bool has_default = false;
		bool has_named = false;
	};

	ItemFactory<peng::weak_ptr<Entity>> _default_factory;
	ItemFactory<peng::weak_ptr<Entity>, const std::string&> _named_factory;
	common::unordered_map<peng::shared_ref<const ReflectedType>, EntityConstructorSet> _type_to_constructor_set;
};

template <typename T>
void EntityFactory::register_entity()
{
	const peng::shared_ref<const ReflectedType> entity_type = ReflectionDatabase::get().reflect_type_checked<T>();
	EntityConstructorSet constructor_set;

	if constexpr (std::is_constructible_v<T>)
	{
		const auto entity_constructor = []() -> peng::weak_ptr<Entity>
		{
			return PengEngine::get().entity_manager().create_entity<T>();
		};

		_default_factory.register_factory(entity_type, entity_constructor);
		constructor_set.has_default = true;
	}

	if constexpr (std::is_constructible_v<T, const std::string&>)
	{
		const auto entity_constructor = [](const std::string& entity_name) -> peng::weak_ptr<Entity>
		{
			return PengEngine::get().entity_manager().create_entity<T>(entity_name);
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
