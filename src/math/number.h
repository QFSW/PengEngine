#pragma once

#include <concepts>

namespace math
{
	template <typename T>
	concept number = std::floating_point<T> || std::integral<T>;

	template <number T>
	struct make_floating
	{
		using type = T;
	};

	template <std::integral T>
	struct make_floating<T>
	{
		using type = float;
	};

	template <number T>
	using make_floating_t = typename make_floating<T>::type;
}