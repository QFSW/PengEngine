#include "entity_factory.h"

#include "entity.h"
#include "archive.h"
#include "component_factory.h"

peng::weak_ptr<Entity> EntityFactory::create_entity(
	const peng::shared_ref<const ReflectedType>& entity_type,
	const std::string& entity_name
) const
{
	if (const auto it = _type_to_constructor_set.find(entity_type); it != _type_to_constructor_set.end())
	{
		const EntityConstructorSet& constructor_set = it->second;

		// If the name is empty, prefer default constructor before named
		if (entity_name.empty())
		{
			if (constructor_set.has_default)
			{
				return _default_factory.construct_item(entity_type);
			}

			if (constructor_set.has_named)
			{
				return _named_factory.construct_item(entity_type, entity_type->name);
			}
		}
		else
		{
			if (constructor_set.has_named)
			{
				return _named_factory.construct_item(entity_type, entity_name);
			}

			if (constructor_set.has_default)
			{
				return _default_factory.construct_item(entity_type);
			}
		}
	}

	Logger::error(
		"Cannot create entity '%s' as type '%s' is not present in the entity factory",
		entity_type->name.c_str(),
		entity_name.c_str()
	);

	return {};
}

peng::weak_ptr<Entity> EntityFactory::load_entity(const Archive& archive)
{
	const std::string entity_type = archive.read_or<std::string>("type");
	if (entity_type.empty())
	{
		Logger::error(
			"Could not load entity '%s' as no type was provided",
			archive.name.c_str()
		);

		return {};
	}

    const peng::shared_ptr<const ReflectedType> reflected_type = ReflectionDatabase::get().reflect_type(entity_type);
	if (!reflected_type)
	{
		Logger::error(
			"Could not load entity '%s' as the type '%s' does not exist",
			archive.name.c_str(), entity_type.c_str()
		);

		return {};
	}

    peng::weak_ptr<Entity> entity = create_entity(reflected_type.to_shared_ref(), archive.name);

	entity->deserialize(archive);
	load_components(archive, entity);

	return entity;
}

void EntityFactory::load_components(const Archive& entity_archive, const peng::weak_ptr<Entity>& entity)
{
	if (const auto it = entity_archive.json_def.find("components"); it != entity_archive.json_def.end())
	{
		if (!it->is_array())
		{
			Logger::error("The 'components' entry in the entity '%s' was not an array", entity->name().c_str());
			return;
		}

		for (const auto& component_def : *it)
		{
			Archive component_archive;
			component_archive.json_def = component_def;

			ComponentFactory::get().load_component(component_archive, entity);
		}
	}
}
