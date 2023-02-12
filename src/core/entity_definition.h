#pragma once

#include "reflection_bootstrap.h"

#define DECLARE_ENTITY(EntityType) \
private: \
	static ReflectionBootstrap<##EntityType> _reflection_bootstrap

#define IMPLEMENT_ENTITY(EntityType) \
	ReflectionBootstrap<##EntityType> EntityType::_reflection_bootstrap(#EntityType)
