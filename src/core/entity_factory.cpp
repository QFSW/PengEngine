#include "entity_factory.h"

peng::weak_ptr<Entity> EntityFactory::create_entity(peng::shared_ref<const ReflectedType> entity_type, const std::string& entity_name) const
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
				return _named_factory.construct_item(entity_type, "NewEntity");
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
