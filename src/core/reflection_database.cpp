#include "reflection_database.h"

void ReflectionDatabase::register_type(const ReflectedType& reflected_type)
{
	check(!reflected_type.name.empty());
	check(!_name_to_type.contains(reflected_type.name));
	check(!_info_to_type.contains(reflected_type.info));

	const peng::shared_ref<ReflectedType> reflected_type_ref = peng::make_shared<ReflectedType>(reflected_type);

	_reflected_types.emplace_back(reflected_type_ref);
	_name_to_type[reflected_type.name] = reflected_type_ref;
	_info_to_type[reflected_type.info] = reflected_type_ref;
}

peng::shared_ptr<const ReflectedType> ReflectionDatabase::reflect_type(const std::type_info& type_info) const
{
	if (const auto it = _info_to_type.find(&type_info); it != _info_to_type.end())
	{
		return it->second;
	}

	return {};
}

peng::shared_ptr<const ReflectedType> ReflectionDatabase::reflect_type(const std::string& type_name) const
{
	if (const auto it = _name_to_type.find(type_name); it != _name_to_type.end())
	{
		return it->second;
	}

	return {};
}

peng::shared_ref<const ReflectedType> ReflectionDatabase::reflect_type_checked(const std::type_info& type_info) const
{
	const peng::shared_ptr<const ReflectedType> reflected_type = reflect_type(type_info);
	check(reflected_type);

	return reflected_type.to_shared_ref();
}

peng::shared_ref<const ReflectedType> ReflectionDatabase::reflect_type_checked(const std::string& type_name) const
{
	const peng::shared_ptr<const ReflectedType> reflected_type = reflect_type(type_name);
	check(reflected_type);

	return reflected_type.to_shared_ref();
}

peng::shared_ptr<const ReflectedType> ReflectionDatabase::resolve_base(
	const peng::shared_ref<const ReflectedType>& derived_type) const
{
	if (const auto it = _info_to_type.find(derived_type->base_info); it != _info_to_type.end())
	{
		return it->second;
	}

	return {};
}

bool ReflectionDatabase::is_derived_from(const std::type_info& derived_type, const std::type_info& base_type)
{
	if (&derived_type == &base_type)
	{
		return true;
	}

	const peng::shared_ref<const ReflectedType> derived_reflected = reflect_type_checked(derived_type);

	if (!derived_reflected->base_info)
	{
		return false;
	}

	return is_derived_from(*derived_reflected->base_info, base_type);
}
