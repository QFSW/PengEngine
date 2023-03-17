#include "rigid_body.h"

IMPLEMENT_COMPONENT(components::RigidBody);

using namespace components;
using namespace math;

RigidBody::RigidBody()
	: Component(TickGroup::physics)
{ }

void RigidBody::tick(float delta_time)
{
	Component::tick(delta_time);

	owner().local_transform().position += velocity * delta_time;
}
