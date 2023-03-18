#include "goal.h"

#include <components/box_collider_2d.h>

#include "paddle.h"

IMPLEMENT_ENTITY(demo::pong::Goal);

using namespace demo::pong;

Goal::Goal()
	: Goal(peng::weak_ptr<Paddle>())
{ }

Goal::Goal(const peng::weak_ptr<Paddle>& associated_paddle)
	: Entity(
		strtools::catf("Goal[%s]", associated_paddle->name().c_str()),
		TickGroup::none
	)
	, _associated_paddle(associated_paddle)
{
	add_component<components::BoxCollider2D>()->layer = physics::Layer(1);
}

const peng::weak_ptr<Paddle>& Goal::associated_paddle() const noexcept
{
	return _associated_paddle;
}
