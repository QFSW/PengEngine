#include "collider_2d.h"

IMPLEMENT_COMPONENT(components::Collider2D);

using namespace components;
using namespace math;

std::vector<peng::weak_ptr<Collider2D>> Collider2D::_active_colliders;

Collider2D::Collider2D()
	: Component(TickGroup::physics)
{ }

const std::vector<peng::weak_ptr<Collider2D>>& Collider2D::active_colliders()
{
	return _active_colliders;
}

void Collider2D::post_create()
{
	Component::post_create();

	_active_colliders.push_back(weak_this());
}

void Collider2D::pre_destroy()
{
	Component::pre_destroy();

	vectools::remove(_active_colliders, weak_this());
}
