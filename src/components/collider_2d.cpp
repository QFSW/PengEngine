#include "collider_2d.h"

IMPLEMENT_COMPONENT(components::Collider2D);

using namespace components;
using namespace math;

Collider2D::Collider2D()
	: Component(TickGroup::physics)
{ }
