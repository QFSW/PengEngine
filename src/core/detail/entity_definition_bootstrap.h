#pragma once

#include <core/entity_factory.h>

#include "reflection_bootstrap.h"

namespace core::detail
{
	template <typename T>
	class EntityDefinitionBootstrap
	{
	public:
		EntityDefinitionBootstrap(const std::string& type_name);
		EntityDefinitionBootstrap(const EntityDefinitionBootstrap&) = delete;
		EntityDefinitionBootstrap(EntityDefinitionBootstrap&&) = delete;

	private:
		ReflectionBootstrap<T> _reflection_bootstrap;
	};

	template <typename T>
	EntityDefinitionBootstrap<T>::EntityDefinitionBootstrap(const std::string& type_name)
		: _reflection_bootstrap(type_name)
	{
		if constexpr (!std::is_abstract_v<T>)
		{
			EntityFactory::get().register_entity<T>();
		}
	}
}