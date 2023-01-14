#include "point_light.h"

#include <core/logger.h>
#include <utils/utils.h>

using namespace entities;

peng::weak_ptr<PointLight> PointLight::_current;

PointLight::PointLight()
	: PointLight("PointLight")
{ }

PointLight::PointLight(const std::string& name)
	: PointLight(utils::copy(name))
{ }

PointLight::PointLight(std::string&& name)
	: Entity(name, TickGroup::none)
{ }

const peng::weak_ptr<PointLight>& PointLight::current()
{
	return _current;
}

void PointLight::post_create()
{
	Entity::post_create();

	if (_current.valid())
	{
		Logger::warning(
			"Cannot register '%s' as a light source ('%s') already exists",
			name().c_str(), _current->name().c_str()
		);
	}
	else
	{
		_current = weak_from(*this);
	}
}
