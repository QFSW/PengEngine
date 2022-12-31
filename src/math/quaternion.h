#pragma once

#include "matrix4x4.h"
#include "vector3.h"

namespace math
{
	template <std::floating_point T>
	struct Quaternion
	{
	public:
		T w;
		T x;
		T y;
		T z;

		Quaternion();
		Quaternion(T w, T x, T y, T z);

		template <std::floating_point U>
		requires std::convertible_to<U, T>
		Quaternion(const Quaternion<U>& other);

		Quaternion& operator+=(const Quaternion& other);
		Quaternion& operator-=(const Quaternion& other);
		Quaternion operator+(const Quaternion& other) const;
		Quaternion operator-(const Quaternion& other) const;
		Quaternion operator*(const Quaternion& other) const;

		[[nodiscard]] Matrix4x4<T> rotator() const noexcept;

		static constexpr Quaternion euler(const Vector3<T>& euler_angles);
		static constexpr Quaternion euler(T yaw, T pitch, T roll);
	};

	template <std::floating_point T>
	constexpr Quaternion<T> Quaternion<T>::euler(const Vector3<T>& euler_angles)
	{
		return euler(euler_angles.x, euler_angles.y, euler_angles.z);
	}

	template <std::floating_point T>
	constexpr Quaternion<T> Quaternion<T>::euler(T yaw, T pitch, T roll)
	{
		const T c = std::numbers::pi_v<T> / 360;
		const T yaw_c = yaw * c;
		const T pitch_c = pitch * c;
		const T roll_c = roll * c;

		return Quaternion(
			std::cos(roll_c) * std::cos(pitch_c) * std::cos(yaw_c) + std::sin(roll_c) * std::sin(pitch_c) * std::sin(yaw_c),
			std::sin(roll_c) * std::cos(pitch_c) * std::cos(yaw_c) - std::cos(roll_c) * std::sin(pitch_c) * std::sin(yaw_c),
			std::cos(roll_c) * std::sin(pitch_c) * std::cos(yaw_c) + std::sin(roll_c) * std::cos(pitch_c) * std::sin(yaw_c),
			std::cos(roll_c) * std::cos(pitch_c) * std::sin(yaw_c) - std::sin(roll_c) * std::sin(pitch_c) * std::cos(yaw_c)
		);
	}

	using QuaternionF = Quaternion<float>;
	using QuaternionD = Quaternion<double>;

	template <std::floating_point T>
	Quaternion<T>::Quaternion()
		: w(0)
		, x(0)
		, y(0)
		, z(0)
	{ }

	template <std::floating_point T>
	Quaternion<T>::Quaternion(T w, T x, T y, T z)
		: w(w)
		, x(x)
		, y(y)
		, z(z)
	{ }

	template <std::floating_point T>
	template <std::floating_point U>
	requires std::convertible_to<U, T>
	Quaternion<T>::Quaternion(const Quaternion<U>& other)
		: w(static_cast<T>(other.w))
		, x(static_cast<T>(other.x))
		, y(static_cast<T>(other.y))
		, z(static_cast<T>(other.z))
	{ }

	template <std::floating_point T>
	Quaternion<T>& Quaternion<T>::operator+=(const Quaternion& other)
	{
		w += other.w;
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	template <std::floating_point T>
	Quaternion<T>& Quaternion<T>::operator-=(const Quaternion& other)
	{
		w -= other.w;
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	template <std::floating_point T>
	Quaternion<T> Quaternion<T>::operator+(const Quaternion& other) const
	{
		Quaternion result = *this;
		return result += other;
	}

	template <std::floating_point T>
	Quaternion<T> Quaternion<T>::operator-(const Quaternion& other) const
	{
		Quaternion result = *this;
		return result -= other;
	}

	template <std::floating_point T>
	Quaternion<T> Quaternion<T>::operator*(const Quaternion& other) const
	{
		const Quaternion& a = *this;
		const Quaternion& b = other;

		return Quaternion(
			a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
			a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
			a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
			a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w
		);
	}

	template <std::floating_point T>
	Matrix4x4<T> Quaternion<T>::rotator() const noexcept
	{
		const float wx = w * x;
		const float wy = w * y;
		const float wz = w * z;

		const float xx = x * x;
		const float xy = x * y;
		const float xz = x * z;

		const float yy = y * y;
		const float yz = y * z;

		const float zz = z * z;

		return Matrix4x4<T>({
			1 - 2 * (yy + zz),  2 * (xy + wz),      2 * (xz - wy),      0,
			2 * (xy - wz),      1 - 2 * (xx + zz),  2 * (yz + wx),      0,
			2 * (xz + wy),      2 * (yz - wx),      1 - 2 * (xx + yy),  0,
			0,                  0,                  0,                  1
		});
	}
}
