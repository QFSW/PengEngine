#pragma once

#include "detail/component_definition_bootstrap.h"

#define DECLARE_COMPONENT(ComponentType) \
public: \
	[[nodiscard]] virtual peng::shared_ref<const ReflectedType> type() const \
	{ \
		return ReflectionDatabase::get().reflect_type_checked<##ComponentType>(); \
	} \
private: \
	static core::detail::ComponentDefinitionBootstrap<##ComponentType> _component_bootstrap

#define IMPLEMENT_COMPONENT(ComponentType) \
	core::detail::ComponentDefinitionBootstrap<##ComponentType> ComponentType::_component_bootstrap(#ComponentType)
