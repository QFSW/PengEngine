#pragma once

#include "detail/entity_definition_bootstrap.h"

#define DECLARE_ENTITY(EntityType) \
public: \
	[[nodiscard]] virtual peng::shared_ref<const ReflectedType> type() const \
	{ \
		return ReflectionDatabase::get().reflect_type_checked<EntityType>(); \
	} \
	\
	[[nodiscard]] peng::weak_ptr<EntityType> weak_this() \
	{ \
		return peng::weak_ptr<EntityType>(std::static_pointer_cast<EntityType>(shared_from_this())); \
	} \
	\
	[[nodiscard]] peng::weak_ptr<const EntityType> weak_this() const \
	{ \
		return peng::weak_ptr<const EntityType>(std::static_pointer_cast<const EntityType>(shared_from_this())); \
	} \
private: \
	static core::detail::EntityDefinitionBootstrap<EntityType> _definition_bootstrap

#define IMPLEMENT_ENTITY(EntityType) \
	core::detail::EntityDefinitionBootstrap<EntityType> EntityType::_definition_bootstrap(#EntityType)
