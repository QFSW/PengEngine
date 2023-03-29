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

	template <typename T, typename U>
	constexpr bool is_losslessly_convertible_to_v()
	{
		return false;
	}

#pragma region is_losslessly_convertible_to_v

	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, uint8_t>()  { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, uint16_t>() { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, uint32_t>() { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, uint64_t>() { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, int8_t>()   { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, int16_t>()  { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, int32_t>()  { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, int64_t>()  { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, float>()    { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<uint8_t, double>()   { return true; }

	template<> constexpr bool is_losslessly_convertible_to_v<int32_t, int32_t>()  { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<int32_t, int64_t>()  { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<int32_t, float>()    { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<int32_t, double>()   { return true; }

	template<> constexpr bool is_losslessly_convertible_to_v<float, float>()      { return true; }
	template<> constexpr bool is_losslessly_convertible_to_v<float, double>()     { return true; }

	template<> constexpr bool is_losslessly_convertible_to_v<double, double>()    { return true; }

#pragma endregion is_losslessly_convertible_to_v

	template <typename T, typename U>
	concept losslessly_convertible_to = is_losslessly_convertible_to_v<T, U>();
}