#include "rigid_body_2d.h"

IMPLEMENT_COMPONENT(components::RigidBody2D);

using namespace components;
using namespace math;

RigidBody2D::RigidBody2D()
	: Component(TickGroup::physics)
{ }

void RigidBody2D::tick(float delta_time)
{
	Component::tick(delta_time);

	owner().local_transform().position += Vector3f(velocity * delta_time, 0);
}
