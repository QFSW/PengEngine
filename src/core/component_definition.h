#pragma once

#include "detail/reflection_bootstrap.h"

// TODO: make components get registered into a factory similar to entities

#define DECLARE_COMPONENT(ComponentType) \
public: \
	[[nodiscard]] virtual peng::shared_ref<const ReflectedType> type() const \
	{ \
		return ReflectionDatabase::get().reflect_type_checked<##ComponentType>(); \
	} \
private: \
	static core::detail::ReflectionBootstrap<##ComponentType> _reflection_bootstrap

#define IMPLEMENT_COMPONENT(ComponentType) \
	core::detail::ReflectionBootstrap<##ComponentType> ComponentType::_reflection_bootstrap(#ComponentType)
