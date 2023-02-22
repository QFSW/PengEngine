#pragma once

#include <string>
#include <typeinfo>

struct ReflectedType
{
	std::string name;
	const std::type_info* info = nullptr;
	const std::type_info* base_info = nullptr;
};
