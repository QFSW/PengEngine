#pragma once

#include <core/component.h>

namespace components
{
	class Collider2D : public Component
	{
		DECLARE_COMPONENT(Collider2D);

	public:
		Collider2D();
	};
}
