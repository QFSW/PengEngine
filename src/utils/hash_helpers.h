#pragma once

#include <tuple>

template<typename...Ts>
struct std::hash<std::tuple<Ts...>>
{
private:
	template <size_t I>
	size_t hash_inner(const std::tuple<Ts...>& tuple) const
	{
		if constexpr (I == sizeof...(Ts))
		{
			return 0;
		}
		else
		{
			const auto& item = std::get<I>(tuple);
			using Ti = std::remove_const_t<std::remove_reference_t<decltype(item)>>;
			size_t hash = std::hash<Ti>{}(item);

			return hash ^= hash_inner<I + 1>(tuple);
		}
	}

public:
	size_t operator()(const std::tuple<Ts...>& tuple) const
	{
		return hash_inner<0>(tuple);
	}
};