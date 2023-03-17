#pragma once

#include "key_code.h"

namespace input
{
	struct Axis
	{
		KeyCode positive = KeyCode::none;
		KeyCode negative = KeyCode::none;

		float magnitude = 1.0f;
	};
}