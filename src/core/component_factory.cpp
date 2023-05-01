#include "component_factory.h"

#include "archive.h"
#include "component.h"

peng::weak_ptr<Component> ComponentFactory::create_component(
	const peng::shared_ref<const ReflectedType>& component_type,
	const peng::weak_ptr<Entity>& entity
) const
{
	return _factory.construct_item(component_type, entity);
}

peng::weak_ptr<Component> ComponentFactory::load_component(const Archive& archive, const peng::weak_ptr<Entity>& entity)
{
	const bool inline_def = archive.json_def.is_string();

	if (!(inline_def || archive.json_def.is_object()))
	{
		Logger::error(
			"Could not load component '%s' on entity '%s' as it is not a component typename or definition",
			archive.json_def.dump().c_str(), entity->name().c_str()
		);

		return {};
	}

	const std::string component_type =
		inline_def
		? archive.json_def.get<std::string>()
		: archive.read_or<std::string>("type");

	if (component_type.empty())
	{
		Logger::error(
			"Could not load component on entity '%s' as no type was provided",
			archive.json_def.dump().c_str(), entity->name().c_str()
		);

		return {};
	}

    const peng::shared_ptr<const ReflectedType> reflected_type = ReflectionDatabase::get().reflect_type(component_type);
	if (!reflected_type)
	{
		Logger::error(
			"Could not load component '%s' on entity '%s' as the type does not exist",
			component_type.c_str(), entity->name().c_str()
		);

		return {};
	}

    peng::weak_ptr<Component> component = create_component(reflected_type.to_shared_ref(), entity);
	if (!inline_def)
	{
		component->deserialize(archive);
	}

	return component;
}
