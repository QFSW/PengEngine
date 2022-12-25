#pragma once

#include <concepts>

namespace math
{
	template <typename T>
	concept number = std::floating_point<T> || std::integral<T>;
}