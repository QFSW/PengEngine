#pragma once

#include "vector2.h"

namespace math
{
	template <number T>
	struct Vector3
	{
	public:
		T x;
		T y;
		T z;

		Vector3();
		Vector3(T x, T y, T z);

#pragma region Swizzling

		Vector3(T x, const Vector2<T>& yz) : x(x), y(yz.x), z(yz.y) { }
		Vector3(const Vector2<T>& xy, T z) : x(xy.x), y(xy.y), z(z) { }

		[[nodiscard]] Vector2<T> xx() const noexcept { return Vector2<T>(x, x); }
		[[nodiscard]] Vector2<T> xy() const noexcept { return Vector2<T>(x, y); }
		[[nodiscard]] Vector2<T> yx() const noexcept { return Vector2<T>(y, x); }
		[[nodiscard]] Vector2<T> yy() const noexcept { return Vector2<T>(y, y); }

		[[nodiscard]] Vector3 xxx() const noexcept { return Vector3<T>(x, x, x); }
		[[nodiscard]] Vector3 xxy() const noexcept { return Vector3<T>(x, x, y); }
		[[nodiscard]] Vector3 xxz() const noexcept { return Vector3<T>(x, x, z); }
		[[nodiscard]] Vector3 xyx() const noexcept { return Vector3<T>(x, y, x); }
		[[nodiscard]] Vector3 xyy() const noexcept { return Vector3<T>(x, y, y); }
		[[nodiscard]] Vector3 xyz() const noexcept { return Vector3<T>(x, y, z); }
		[[nodiscard]] Vector3 xzx() const noexcept { return Vector3<T>(x, z, x); }
		[[nodiscard]] Vector3 xzy() const noexcept { return Vector3<T>(x, z, y); }
		[[nodiscard]] Vector3 xzz() const noexcept { return Vector3<T>(x, z, z); }
		[[nodiscard]] Vector3 yxx() const noexcept { return Vector3<T>(y, x, x); }
		[[nodiscard]] Vector3 yxy() const noexcept { return Vector3<T>(y, x, y); }
		[[nodiscard]] Vector3 yxz() const noexcept { return Vector3<T>(y, x, z); }
		[[nodiscard]] Vector3 yyx() const noexcept { return Vector3<T>(y, y, x); }
		[[nodiscard]] Vector3 yyy() const noexcept { return Vector3<T>(y, y, y); }
		[[nodiscard]] Vector3 yyz() const noexcept { return Vector3<T>(y, y, z); }
		[[nodiscard]] Vector3 yzx() const noexcept { return Vector3<T>(y, z, x); }
		[[nodiscard]] Vector3 yzy() const noexcept { return Vector3<T>(y, z, y); }
		[[nodiscard]] Vector3 yzz() const noexcept { return Vector3<T>(y, z, z); }
		[[nodiscard]] Vector3 zxx() const noexcept { return Vector3<T>(z, x, x); }
		[[nodiscard]] Vector3 zxy() const noexcept { return Vector3<T>(z, x, y); }
		[[nodiscard]] Vector3 zxz() const noexcept { return Vector3<T>(z, x, z); }
		[[nodiscard]] Vector3 zyx() const noexcept { return Vector3<T>(z, y, x); }
		[[nodiscard]] Vector3 zyy() const noexcept { return Vector3<T>(z, y, y); }
		[[nodiscard]] Vector3 zyz() const noexcept { return Vector3<T>(z, y, z); }
		[[nodiscard]] Vector3 zzx() const noexcept { return Vector3<T>(z, z, x); }
		[[nodiscard]] Vector3 zzy() const noexcept { return Vector3<T>(z, z, y); }
		[[nodiscard]] Vector3 zzz() const noexcept { return Vector3<T>(z, z, z); }

#pragma endregion

		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3 operator+(const Vector3& other);
		Vector3 operator-(const Vector3& other);
	};

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3i = Vector3<int32_t>;
	using Vector3u = Vector3<uint32_t>;

	template <number T>
	Vector3<T>::Vector3()
		: x(0)
		, y(0)
		, z(0)
	{ }

	template <number T>
	Vector3<T>::Vector3(T x, T y, T z)
		: x(x)
		, y(y)
		, z(z)
	{ }

	template <number T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	template <number T>
	Vector3<T>& Vector3<T>::operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	template <number T>
	Vector3<T> Vector3<T>::operator+(const Vector3& other)
	{
		Vector3 result = *this;
		return result += other;
	}

	template <number T>
	Vector3<T> Vector3<T>::operator-(const Vector3& other)
	{
		Vector3 result = *this;
		return result -= other;
	}
}