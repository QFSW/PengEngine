#pragma once

#include "detail/entity_definition_bootstrap.h"

#define DECLARE_ENTITY(EntityType) \
private: \
	static core::detail::EntityDefinitionBootstrap<##EntityType> _definition_bootstrap

#define IMPLEMENT_ENTITY(EntityType) \
	core::detail::EntityDefinitionBootstrap<##EntityType> EntityType::_definition_bootstrap(#EntityType)
