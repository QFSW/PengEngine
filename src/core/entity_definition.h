#pragma once

#include "detail/entity_definition_bootstrap.h"

#define DECLARE_ENTITY(EntityType) \
public: \
	[[nodiscard]] virtual peng::shared_ref<const ReflectedType> type() const \
	{ \
		return ReflectionDatabase::get().reflect_type_checked<##EntityType>(); \
	} \
private: \
	static core::detail::EntityDefinitionBootstrap<##EntityType> _definition_bootstrap

#define IMPLEMENT_ENTITY(EntityType) \
	core::detail::EntityDefinitionBootstrap<##EntityType> EntityType::_definition_bootstrap(#EntityType)
