#include "spot_light.h"

#include <core/serialized_member.h>
#include <utils/vectools.h>
#include <utils/utils.h>

IMPLEMENT_ENTITY(entities::SpotLight);

namespace entities
{
	// TODO: this (and other light data) is non ideal as unfilled fields will be defaulted
	//       instead of being left untouched
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
		SpotLight::LightData,
		color, ambient, range, umbra, penumbra
	);
}

using namespace entities;

std::vector<peng::weak_ptr<SpotLight>> SpotLight::_active_lights;

SpotLight::SpotLight()
	: SpotLight("SpotLight")
{ }

SpotLight::SpotLight(const std::string& name)
	: SpotLight(utils::copy(name))
{ }

SpotLight::SpotLight(std::string&& name)
	: Entity(name, TickGroup::none)
	, _data({
		math::Vector3f::one(),
		math::Vector3f::one() * 0.01f,
		10
	})
{
	SERIALIZED_MEMBER(_data);
}

const std::vector<peng::weak_ptr<SpotLight>>& SpotLight::active_lights()
{
	return _active_lights;
}

void SpotLight::post_create()
{
	Entity::post_create();

	_active_lights.push_back(weak_this());
}

void SpotLight::pre_destroy()
{
	Entity::pre_destroy();

	vectools::remove(_active_lights, weak_this());
}

SpotLight::LightData& SpotLight::data() noexcept
{
	return _data;
}

const SpotLight::LightData& SpotLight::data() const noexcept
{
	return _data;
}
