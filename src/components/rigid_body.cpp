#include "rigid_body.h"

#include <core/serialized_member.h>

IMPLEMENT_COMPONENT(components::RigidBody);

using namespace components;
using namespace math;

RigidBody::RigidBody()
	: Component(TickGroup::physics)
{
	SERIALIZED_MEMBER(velocity);
}

void RigidBody::tick(float delta_time)
{
	Component::tick(delta_time);

	owner().local_transform().position += velocity * delta_time;
}
