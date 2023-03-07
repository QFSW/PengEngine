#pragma once

#include <memory/weak_ptr.h>

#include "entity.h"
#include "logger.h"
#include "item_factory.h"
#include "reflection_database.h"

class Component;

class ComponentFactory : public utils::Singleton<ComponentFactory>
{
	using Singleton::Singleton;

public:
	template <std::derived_from<Component> T>
	void register_component();

	// NOLINT(modernize-use-nodiscard)
	peng::weak_ptr<Component> create_component(
		const peng::shared_ref<const ReflectedType>& component_type,
		const peng::weak_ptr<Entity>& entity
	) const;

private:
	ItemFactory<peng::weak_ptr<Component>, const peng::weak_ptr<Entity>&> _factory;
};

template <std::derived_from<Component> T>
void ComponentFactory::register_component()
{
	const peng::shared_ref<const ReflectedType> component_type = ReflectionDatabase::get().reflect_type_checked<T>();

	if constexpr (std::is_constructible_v<T>)
	{
		const auto component_constructor = [](const peng::weak_ptr<Entity>& entity) -> peng::weak_ptr<Component>
		{
			return entity->add_component<T>();
		};

		_factory.register_factory(component_type, component_constructor);
	}
	else
	{
		Logger::warning(
			"Component type '%s' cannot be registered to the component factory due to missing all viable constructors",
			component_type->name.c_str()
		);
	}
}
