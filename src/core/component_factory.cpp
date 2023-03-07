#include "component_factory.h"

peng::weak_ptr<Component> ComponentFactory::create_component(
	const peng::shared_ref<const ReflectedType>& component_type,
	const peng::weak_ptr<Entity>& entity
) const
{
	return _factory.construct_item(component_type, entity);
}
