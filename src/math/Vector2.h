#pragma once

#include <algorithm>
#include <cmath>

#include "number.h"

namespace math
{
	template <number T>
	struct Vector2
	{
	public:
		using F = make_floating_t<T>;

		T x;
		T y;

		Vector2();
		Vector2(T x, T y);

		template <number U>
		requires std::convertible_to<U, T>
		Vector2(const Vector2<U>& other);

#pragma region Swizzling

		[[nodiscard]] Vector2 xx() const noexcept { return Vector2<T>(x, x); }
		[[nodiscard]] Vector2 xy() const noexcept { return Vector2<T>(x, y); }
		[[nodiscard]] Vector2 yx() const noexcept { return Vector2<T>(y, x); }
		[[nodiscard]] Vector2 yy() const noexcept { return Vector2<T>(y, y); }

#pragma endregion

		[[nodiscard]] T min() const noexcept;
		[[nodiscard]] T max() const noexcept;
		[[nodiscard]] T magnitude_sqr() const noexcept;
		[[nodiscard]] F magnitude() const noexcept;
		[[nodiscard]] Vector2<F> normalized() const noexcept;
		[[nodiscard]] Vector2<F> normalized_unsafe() const noexcept;
		[[nodiscard]] Vector2<F> reciprocal() const noexcept;

		Vector2 operator+() const;
		Vector2 operator-() const;

		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator*=(const Vector2& other);
		Vector2& operator/=(const Vector2& other);
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(const Vector2& other) const;
		Vector2 operator/(const Vector2& other) const;

		Vector2& operator*=(const T& scalar);
		Vector2& operator/=(const T& scalar);
		Vector2 operator*(const T& scalar) const;
		Vector2 operator/(const T& scalar) const;

		static constexpr Vector2 zero() { return Vector2(0, 0); }
		static constexpr Vector2 one() { return Vector2(1, 1); }
	};

	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;
	using Vector2i = Vector2<int32_t>;
	using Vector2u = Vector2<uint32_t>;
	using Vector2u8 = Vector2<uint8_t>;

	template <number T>
	Vector2<T>::Vector2()
		: x(0)
		, y(0)
	{ }

	template <number T>
	Vector2<T>::Vector2(T x, T y)
		: x(x)
		, y(y)
	{ }

	template <number T>
	template <number U>
	requires std::convertible_to<U, T>
	Vector2<T>::Vector2(const Vector2<U>& other)
		: x(static_cast<T>(other.x))
		, y(static_cast<T>(other.y))
	{ }

	template <number T>
	T Vector2<T>::min() const noexcept
	{
		return std::min(x, y);
	}

	template <number T>
	T Vector2<T>::max() const noexcept
	{
		return std::max(x, y);
	}

	template <number T>
	T Vector2<T>::magnitude_sqr() const noexcept
	{
		return x * x + y * y;
	}

	template <number T>
	make_floating_t<T> Vector2<T>::magnitude() const noexcept
	{
		return std::sqrt(static_cast<F>(magnitude_sqr()));
	}

	template <number T>
	Vector2<make_floating_t<T>> Vector2<T>::normalized() const noexcept
	{
		const F mag = magnitude();
		if (mag != 0)
		{
			return static_cast<Vector2<F>>(*this) / mag;
		}

		return *this;
	}

	template <number T>
	Vector2<make_floating_t<T>> Vector2<T>::normalized_unsafe() const noexcept
	{
		return static_cast<Vector2<F>>(*this) / magnitude();
	}

	template <number T>
	Vector2<make_floating_t<T>> Vector2<T>::reciprocal() const noexcept
	{
		constexpr F one_f = 1;
		return Vector2<F>(one_f / x, one_f / y);
	}

	template <number T>
	Vector2<T> Vector2<T>::operator+() const
	{
		return *this;
	}

	template <number T>
	Vector2<T> Vector2<T>::operator-() const
	{
		return *this * -1;
	}

	template <number T>
	Vector2<T>& Vector2<T>::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	template <number T>
	Vector2<T>& Vector2<T>::operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	template <number T>
	Vector2<T>& Vector2<T>::operator*=(const Vector2& other)
	{
		x *= other.x;
		y *= other.y;

		return *this;
	}

	template <number T>
	Vector2<T>& Vector2<T>::operator/=(const Vector2& other)
	{
		x /= other.x;
		y /= other.y;

		return* this;
	}

	template <number T>
	Vector2<T> Vector2<T>::operator+(const Vector2& other) const
	{
		Vector2 result = *this;
		return result += other;
	}

	template <number T>
	Vector2<T> Vector2<T>::operator-(const Vector2& other) const
	{
		Vector2 result = *this;
		return result -= other;
	}

	template <number T>
	Vector2<T> Vector2<T>::operator*(const Vector2& other) const
	{
		Vector2 result = *this;
		return result *= other;
	}

	template <number T>
	Vector2<T> Vector2<T>::operator/(const Vector2& other) const
	{
		Vector2 result = *this;
		return result /= other;
	}

	template <number T>
	Vector2<T>& Vector2<T>::operator*=(const T& scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	template <number T>
	Vector2<T>& Vector2<T>::operator/=(const T& scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	template <number T>
	Vector2<T> Vector2<T>::operator*(const T& scalar) const
	{
		Vector2 result = *this;
		return result *= scalar;
	}

	template <number T>
	Vector2<T> Vector2<T>::operator/(const T& scalar) const
	{
		Vector2 result = *this;
		return result /= scalar;
	}
}
