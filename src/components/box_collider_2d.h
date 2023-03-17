#pragma once

#include "collider_2d.h"

namespace components
{
	class BoxCollider2D : public Collider2D
	{
		DECLARE_COMPONENT(BoxCollider2D);

	public:
		using Collider2D::Collider2D;

		physics::AABB bounding_box() const override;
	};
}
