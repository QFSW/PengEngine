#pragma once

#include <concepts>
#include <variant>

namespace utils
{
	template <typename T, typename TVariant>
	concept variant_member =
        std::constructible_from<TVariant, T>
        && !std::same_as<T, TVariant>
        && requires(const TVariant& v)
    {
        std::get<T>(v);
    };
}
