#include "reflection_database.h"

ReflectionDatabase& ReflectionDatabase::get()
{
	static ReflectionDatabase reflection_database;
	return reflection_database;
}

void ReflectionDatabase::register_type(const ReflectedType& reflected_type)
{
	assert(reflected_type.name);
	assert(_name_to_type.contains(reflected_type.name));
	assert(_info_to_type.contains(reflected_type.info));

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

peng::shared_ptr<const ReflectedType> ReflectionDatabase::resolve_base(
	const peng::shared_ref<const ReflectedType>& derived_type) const
{
	if (const auto it = _info_to_type.find(derived_type->base_info); it != _info_to_type.end())
	{
		return it->second;
	}

	return {};
}
