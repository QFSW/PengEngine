#pragma once

#include "detail/component_definition_bootstrap.h"

#define DECLARE_COMPONENT(ComponentType) \
public: \
	[[nodiscard]] virtual peng::shared_ref<const ReflectedType> type() const \
	{ \
		return ReflectionDatabase::get().reflect_type_checked<ComponentType>(); \
	} \
	\
	[[nodiscard]] peng::weak_ptr<ComponentType> weak_this() \
	{ \
		return peng::weak_ptr<ComponentType>(std::static_pointer_cast<ComponentType>(shared_from_this())); \
	} \
	\
	[[nodiscard]] peng::weak_ptr<const ComponentType> weak_this() const \
	{ \
		return peng::weak_ptr<const ComponentType>(std::static_pointer_cast<const ComponentType>(shared_from_this())); \
	} \
private: \
	static core::detail::ComponentDefinitionBootstrap<ComponentType> _component_bootstrap

#define IMPLEMENT_COMPONENT(ComponentType) \
	core::detail::ComponentDefinitionBootstrap<ComponentType> ComponentType::_component_bootstrap(#ComponentType)
