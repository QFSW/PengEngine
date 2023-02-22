#pragma once

#include <vector>

#include <common/common.h>
#include <memory/shared_ptr.h>

#include "reflected_type.h"

class ReflectionDatabase
{
public:
	static ReflectionDatabase& get();

	ReflectionDatabase(const ReflectionDatabase&) = delete;
	ReflectionDatabase(ReflectionDatabase&&) = delete;

	void register_type(const ReflectedType& reflected_type);

	[[nodiscard]] peng::shared_ptr<const ReflectedType> reflect_type(const std::type_info& type_info) const;
	[[nodiscard]] peng::shared_ptr<const ReflectedType> reflect_type(const std::string& type_name) const;
	[[nodiscard]] peng::shared_ref<const ReflectedType> reflect_type_checked(const std::type_info& type_info) const;
	[[nodiscard]] peng::shared_ref<const ReflectedType> reflect_type_checked(const std::string& type_name) const;
	[[nodiscard]] peng::shared_ptr<const ReflectedType> resolve_base(
		const peng::shared_ref<const ReflectedType>& derived_type) const;

	template <typename T>
	[[nodiscard]] peng::shared_ptr<const ReflectedType> reflect_type() const;

	template <typename T>
	[[nodiscard]] peng::shared_ptr<const ReflectedType> resolve_base() const;

	[[nodiscard]] bool is_derived_from(const std::type_info& derived_type, const std::type_info& base_type);

private:
	ReflectionDatabase() = default;

	std::vector<peng::shared_ptr<ReflectedType>> _reflected_types;
	common::unordered_map<std::string, peng::shared_ptr<ReflectedType>> _name_to_type;
	common::unordered_map<const std::type_info*, peng::shared_ptr<ReflectedType>> _info_to_type;
};

template <typename T>
peng::shared_ptr<const ReflectedType> ReflectionDatabase::reflect_type() const
{
	return reflect_type(typeid(T));
}

template <typename T>
peng::shared_ptr<const ReflectedType> ReflectionDatabase::resolve_base() const
{
	if (const peng::shared_ptr<const ReflectedType> derived_type = reflect_type<T>())
	{
		return resolve_base(derived_type.to_shared_ref());
	}

	return {};
}
