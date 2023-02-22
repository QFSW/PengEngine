#pragma once

#include <core/reflection_database.h>

namespace core::detail
{
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
		Logger::log("Registering reflection information for '%s'", type_name.c_str());

		ReflectedType type;
		type.name = type_name;
		type.info = &typeid(T);

		ReflectionDatabase::get().register_type(type);
	}
}
