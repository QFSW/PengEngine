#include "point_light.h"

#include <utils/vectools.h>
#include <utils/utils.h>

IMPLEMENT_ENTITY(entities::PointLight);

using namespace entities;

std::vector<peng::weak_ptr<PointLight>> PointLight::_active_lights;

PointLight::PointLight()
	: PointLight("PointLight")
{ }

PointLight::PointLight(const std::string& name)
	: PointLight(utils::copy(name))
{ }

PointLight::PointLight(std::string&& name)
	: Entity(name, TickGroup::none)
	, _data({
		math::Vector3f::one(),
		math::Vector3f::one() * 0.01f,
		10
	})
{ }

const std::vector<peng::weak_ptr<PointLight>>& PointLight::active_lights()
{
	return _active_lights;
}

void PointLight::post_create()
{
	Entity::post_create();

	_active_lights.push_back(weak_this());
}

void PointLight::pre_destroy()
{
	Entity::pre_destroy();

	vectools::remove(_active_lights, weak_this());
}

PointLight::LightData& PointLight::data() noexcept
{
	return _data;
}

const PointLight::LightData& PointLight::data() const noexcept
{
	return _data;
}
