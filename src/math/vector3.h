#pragma once

#include "number.h"

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

		Vector3 xxx() const noexcept { return Vector3<T>(x, x, x); }
		Vector3 xxy() const noexcept { return Vector3<T>(x, x, y); }
		Vector3 xxz() const noexcept { return Vector3<T>(x, x, z); }
		Vector3 xyx() const noexcept { return Vector3<T>(x, y, x); }
		Vector3 xyy() const noexcept { return Vector3<T>(x, y, y); }
		Vector3 xyz() const noexcept { return Vector3<T>(x, y, z); }
		Vector3 xzx() const noexcept { return Vector3<T>(x, z, x); }
		Vector3 xzy() const noexcept { return Vector3<T>(x, z, y); }
		Vector3 xzz() const noexcept { return Vector3<T>(x, z, z); }
		Vector3 yxx() const noexcept { return Vector3<T>(y, x, x); }
		Vector3 yxy() const noexcept { return Vector3<T>(y, x, y); }
		Vector3 yxz() const noexcept { return Vector3<T>(y, x, z); }
		Vector3 yyx() const noexcept { return Vector3<T>(y, y, x); }
		Vector3 yyy() const noexcept { return Vector3<T>(y, y, y); }
		Vector3 yyz() const noexcept { return Vector3<T>(y, y, z); }
		Vector3 yzx() const noexcept { return Vector3<T>(y, z, x); }
		Vector3 yzy() const noexcept { return Vector3<T>(y, z, y); }
		Vector3 yzz() const noexcept { return Vector3<T>(y, z, z); }
		Vector3 zxx() const noexcept { return Vector3<T>(z, x, x); }
		Vector3 zxy() const noexcept { return Vector3<T>(z, x, y); }
		Vector3 zxz() const noexcept { return Vector3<T>(z, x, z); }
		Vector3 zyx() const noexcept { return Vector3<T>(z, y, x); }
		Vector3 zyy() const noexcept { return Vector3<T>(z, y, y); }
		Vector3 zyz() const noexcept { return Vector3<T>(z, y, z); }
		Vector3 zzx() const noexcept { return Vector3<T>(z, z, x); }
		Vector3 zzy() const noexcept { return Vector3<T>(z, z, y); }
		Vector3 zzz() const noexcept { return Vector3<T>(z, z, z); }

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