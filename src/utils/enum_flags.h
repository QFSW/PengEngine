#pragma once

#include <type_traits>

#define ENUM_FLAGS(Enum) \
	constexpr Enum operator&(Enum a, Enum b) \
	{ \
		return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(a) & static_cast<std::underlying_type_t<Enum>>(b)); \
	} \
	constexpr Enum operator|(Enum a, Enum b) \
	{ \
		return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(a) | static_cast<std::underlying_type_t<Enum>>(b)); \
	} \
	constexpr Enum operator^(Enum a, Enum b) \
	{ \
		return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(a) ^ static_cast<std::underlying_type_t<Enum>>(b)); \
	} 
