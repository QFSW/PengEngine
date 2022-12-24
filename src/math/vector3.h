#pragma once

#include <concepts>

namespace math
{
	template <typename T>
	concept number = std::floating_point<T> || std::integral<T>;

	template <number T>
	struct Vector3
	{
	public:
		T x;
		T y;
		T z;

		Vector3();
		Vector3(T x, T y, T z);

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