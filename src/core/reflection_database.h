#pragma once

#include <string>
#include <vector>
#include <typeinfo>

#include <common/common.h>
#include <memory/shared_ptr.h>

struct ReflectedType
{
	std::string name;
	const std::type_info* info = nullptr;
	const std::type_info* base_info = nullptr;
};

class ReflectionDatabase
{
public:
	static ReflectionDatabase& get();

	ReflectionDatabase(const ReflectionDatabase&) = delete;
	ReflectionDatabase(ReflectionDatabase&&) = delete;

	void register_type(const ReflectedType& reflected_type);
	[[nodiscard]] peng::shared_ptr<ReflectedType> reflect_type(const std::type_info& type_info) const;

	template <typename T>
	[[nodiscard]] peng::shared_ptr<ReflectedType> reflect_type() const;

private:
	ReflectionDatabase() = default;

	std::vector<peng::shared_ptr<ReflectedType>> _reflected_types;
	common::unordered_map<std::string, peng::shared_ptr<ReflectedType>> _name_to_type;
	common::unordered_map<const std::type_info*, peng::shared_ptr<ReflectedType>> _info_to_type;
};

template <typename T>
peng::shared_ptr<ReflectedType> ReflectionDatabase::reflect_type() const
{
	return reflect_type(typeid(T));
}
