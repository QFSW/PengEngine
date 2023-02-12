#pragma once

#include "reflection_database.h"

// TODO: add base class info to reflection bootstrap
template <typename T>
class ReflectionBootstrap
{
public:
	ReflectionBootstrap(const std::string& type_name);
	ReflectionBootstrap(const ReflectionBootstrap&) = delete;
	ReflectionBootstrap(ReflectionBootstrap&&) = delete;
};

template <typename T>
ReflectionBootstrap<T>::ReflectionBootstrap(const std::string& type_name)
{
	ReflectedType type;
	type.name = type_name;
	type.info = &typeid(T);

	ReflectionDatabase::get().register_type(type);
}
