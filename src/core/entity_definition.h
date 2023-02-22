#pragma once

#include "reflection_bootstrap.h"

// TODO: Register a factory for this entity in the ItemFactory

#define DECLARE_ENTITY(EntityType) \
private: \
	static ReflectionBootstrap<##EntityType> _reflection_bootstrap

#define IMPLEMENT_ENTITY(EntityType) \
	ReflectionBootstrap<##EntityType> EntityType::_reflection_bootstrap(#EntityType)
