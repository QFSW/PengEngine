#pragma once

#include <cmath>
#include <numbers>

#include "matrix.h"
#include "vector4.h"
#include "quaternion.h"

namespace math
{
	template <number T>
	struct Matrix4x4 : public Matrix<T, 4, 4>
	{
	public:
		using F = make_floating_t<T>;

		Matrix4x4();
		Matrix4x4(const Matrix<T, 4, 4>& m);
		explicit Matrix4x4(const std::array<T, 16>& elements);

		// Returns a scaled copied of the matrix
		// Equivalent to but faster than 'from_scale(scale) * m'
		[[nodiscard]] Matrix4x4 scaled(const Vector3<T>& scale) const noexcept;

		// Returns a translated copied of the matrix
		// Equivalent to but faster than 'from_translation(scale) * m'
		[[nodiscard]] Matrix4x4 translated(const Vector3<T>& translation) const noexcept;

		// Returns a rotated copied of the matrix
		[[nodiscard]] Matrix4x4<F> rotated(const Vector3<F>& rotation) const noexcept;
		[[nodiscard]] Matrix4x4<F> rotated(const Quaternion<F>& rotation) const noexcept;

		[[nodiscard]] Vector3<T> get_translation() const noexcept;

		Vector3<T> operator*(const Vector3<T>& other) const;
		Vector4<T> operator*(const Vector4<T>& other) const;

		static constexpr Matrix4x4 identity();
		static constexpr Matrix4x4 from_translation(const Vector3<T>& translation);
		static constexpr Matrix4x4 from_scale(const Vector3<T>& scale);
		static constexpr Matrix4x4 from_rotation(const Vector3<F>& rotation);
	};

	using Matrix4x4f = Matrix4x4<float>;
	using Matrix4x4d = Matrix4x4<double>;
	using Matrix4x4i = Matrix4x4<int32_t>;
	using Matrix4x4u = Matrix4x4<uint32_t>;

	template <number T>
	constexpr Matrix4x4<T> Matrix4x4<T>::identity()
	{
		return Matrix4x4({
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		});
	}

	template <number T>
	constexpr Matrix4x4<T> Matrix4x4<T>::from_translation(const Vector3<T>& translation)
	{
		const Vector3<T>& t = translation;
		return Matrix4x4({
			1,   0,   0,   0,
			0,	 1,   0,   0,
			0,   0,   1,   0,
			t.x, t.y, t.z, 1
		});
	}

	template <number T>
	constexpr Matrix4x4<T> Matrix4x4<T>::from_scale(const Vector3<T>& scale)
	{
		const Vector3<T>& s = scale;
		return Matrix4x4({
			s.x, 0,   0,   0,
			0,   s.y, 0,   0,
			0,   0,   s.z, 0,
			0,   0,   0,   1
		});
	}

    template <number T>
    constexpr Matrix4x4<T> Matrix4x4<T>::from_rotation(const Vector3<F>& rotation)
    {
		// x = pitch, y = yaw, z = roll
		const Vector3<F> r = rotation * std::numbers::pi_v<T> / 180;

		const F sx = std::sin(r.x);
		const F sy = std::sin(r.y);
		const F sz = std::sin(r.z);
		const F cx = std::cos(r.x);
		const F cy = std::cos(r.y);
		const F cz = std::cos(r.z);

		return Matrix4x4<F>({
			cy * cz - sx * sy * sz,  cy * sz + sx * sy * cz,  -cx * sy,  0,
			-cx * sz,                cx * cz,                 sx,        0,
			sx * cy * sz + sy * cz,  sy * sz - sx * cy * cz,  cx * cy,   0,
			0,                       0,                       0,         1
		});
    }

    template <number T>
	Matrix4x4<T>::Matrix4x4()
		: Matrix<T, 4, 4>()
	{ }

	template <number T>
	Matrix4x4<T>::Matrix4x4(const Matrix<T, 4, 4>& m)
		: Matrix<T, 4, 4>(m)
	{ }

	template <number T>
	Matrix4x4<T>::Matrix4x4(const std::array<T, 16>& elements)
		: Matrix<T, 4, 4>(elements)
	{ }

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::scaled(const Vector3<T>& scale) const noexcept
	{
		Matrix4x4 m = *this;
		m.elements[0]  *= scale.x;
		m.elements[4]  *= scale.x;
		m.elements[8]  *= scale.x;
		m.elements[12] *= scale.x;
		m.elements[1]  *= scale.y;
		m.elements[5]  *= scale.y;
		m.elements[9]  *= scale.y;
		m.elements[13] *= scale.y;
		m.elements[2]  *= scale.z;
		m.elements[6]  *= scale.z;
		m.elements[10] *= scale.z;
		m.elements[14] *= scale.z;

		return m;
	}

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::translated(const Vector3<T>& translation) const noexcept
	{
		Matrix4x4 m = *this;
		m.elements[12] += translation.x;
		m.elements[13] += translation.y;
		m.elements[14] += translation.z;

		return m;
	}

	template <number T>
	Matrix4x4<make_floating_t<T>> Matrix4x4<T>::rotated(const Vector3<F>& rotation) const noexcept
	{
		// Skip applying rotation if it's sufficiently small
		if (rotation.magnitude_sqr() < 0.01f)
		{
			return *this;
		}

		// x = pitch, y = yaw, z = roll
		return from_rotation(rotation) * *this;
	}

	template <number T>
	Matrix4x4<make_floating_t<T>> Matrix4x4<T>::rotated(const Quaternion<F>& rotation) const noexcept
	{
		const Quaternion<F>& r = rotation;

		const float wx = r.w * r.x;
		const float wy = r.w * r.y;
		const float wz = r.w * r.z;

		const float xx = r.x * r.x;
		const float xy = r.x * r.y;
		const float xz = r.x * r.z;

		const float yy = r.y * r.y;
		const float yz = r.y * r.z;

		const float zz = r.z * r.z;

		const Matrix4x4<F> m({
			1 - 2 * (yy + zz),  2 * (xy + wz),      2 * (xz - wy),      0,
			2 * (xy - wz),      1 - 2 * (xx + zz),  2 * (yz + wx),      0,
			2 * (xz + wy),      2 * (yz - wx),      1 - 2 * (xx + yy),  0,
			0,                  0,                  0,                  1
		});

		return m * *this;
	}

	template <number T>
	Vector3<T> Matrix4x4<T>::get_translation() const noexcept
	{
		return Vector3<T>(this->elements[12], this->elements[13], this->elements[14]);
	}

	template <number T>
	Vector3<T> Matrix4x4<T>::operator*(const Vector3<T>& other) const
	{
		return (*this * Vector4<T>(other, 1)).xyz();
	}

	template <number T>
	Vector4<T> Matrix4x4<T>::operator*(const Vector4<T>& other) const
	{
		return Vector4<T>(
			other.x * this->get(0, 0) + other.y * this->get(0, 1) + other.z * this->get(0, 2) + other.w * this->get(0, 3),
			other.x * this->get(1, 0) + other.y * this->get(1, 1) + other.z * this->get(1, 2) + other.w * this->get(1, 3),
			other.x * this->get(2, 0) + other.y * this->get(2, 1) + other.z * this->get(2, 2) + other.w * this->get(2, 3),
			other.x * this->get(3, 0) + other.y * this->get(3, 1) + other.z * this->get(3, 2) + other.w * this->get(3, 3)
		);
	}
}
