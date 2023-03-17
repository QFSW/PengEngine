#include "directional_light.h"

#include <core/logger.h>
#include <utils/utils.h>

IMPLEMENT_ENTITY(entities::DirectionalLight);

using namespace entities;

peng::weak_ptr<DirectionalLight> DirectionalLight::_current;

DirectionalLight::DirectionalLight()
	: DirectionalLight("DirectionalLight")
{ }

DirectionalLight::DirectionalLight(const std::string& name)
	: DirectionalLight(utils::copy(name))
{ }

DirectionalLight::DirectionalLight(std::string&& name)
	: Entity(name, TickGroup::none)
	, _data({
		math::Vector3f::one(),
		math::Vector3f::one() * 0.01f,
	})
{ }

const peng::weak_ptr<DirectionalLight>& DirectionalLight::current()
{
	return _current;
}

void DirectionalLight::post_create()
{
	Entity::post_create();

	if (_current.valid())
	{
		Logger::warning("Only one directional light can be used at a time");
	}

	_current = weak_this();
	assert(_current);
}

DirectionalLight::LightData& DirectionalLight::data() noexcept
{
	return _data;
}

const DirectionalLight::LightData& DirectionalLight::data() const noexcept
{
	return _data;
}
