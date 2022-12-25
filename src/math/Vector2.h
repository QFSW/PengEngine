#pragma once

#include <algorithm>

#include "number.h"

namespace math
{
	template <number T>
	struct Vector2
	{
	public:
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

		T min() const noexcept;
		T max() const noexcept;

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
