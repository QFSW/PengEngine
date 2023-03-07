#pragma once

#include <core/component_factory.h>

#include "reflection_bootstrap.h"

namespace core::detail
{
	template <typename T>
	class ComponentDefinitionBootstrap
	{
	public:
		ComponentDefinitionBootstrap(const std::string& type_name);
		ComponentDefinitionBootstrap(const ComponentDefinitionBootstrap&) = delete;
		ComponentDefinitionBootstrap(ComponentDefinitionBootstrap&&) = delete;

	private:
		ReflectionBootstrap<T> _reflection_bootstrap;
	};

	template <typename T>
	ComponentDefinitionBootstrap<T>::ComponentDefinitionBootstrap(const std::string& type_name)
		: _reflection_bootstrap(type_name)
	{
		if constexpr (!std::is_abstract_v<T>)
		{
			ComponentFactory::get().register_component<T>();
		}
	}
}