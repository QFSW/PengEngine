#pragma once

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

#pragma region Swizzling

		Vector2 xx() const noexcept { return Vector2<T>(x, x); }
		Vector2 xy() const noexcept { return Vector2<T>(x, y); }
		Vector2 yx() const noexcept { return Vector2<T>(y, x); }
		Vector2 yy() const noexcept { return Vector2<T>(y, y); }

#pragma endregion

		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2 operator+(const Vector2& other);
		Vector2 operator-(const Vector2& other);
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
	Vector2<T> Vector2<T>::operator+(const Vector2& other)
	{
		Vector2 result = *this;
		return result += other;
	}

	template <number T>
	Vector2<T> Vector2<T>::operator-(const Vector2& other)
	{
		Vector2 result = *this;
		return result -= other;
	}
}