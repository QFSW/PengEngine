#pragma once

#include <array>
#include <cmath>
#include <numbers>

#include "vector4.h"
#include "quaternion.h"

namespace math
{
	template <number T>
	struct Matrix4x4
	{
	public:
		using F = make_floating_t<T>;

		std::array<T, 16> elements;

		Matrix4x4();
		Matrix4x4(const std::array<T, 16>& elements);

		template <number U>
		requires std::convertible_to<U, T>
		Matrix4x4(const Matrix4x4<U>& other);

		[[nodiscard]] T& get(uint8_t row, uint8_t col);
		[[nodiscard]] const T& get(uint8_t row, uint8_t col) const;

		[[nodiscard]] Matrix4x4 transposed() const noexcept;
		[[nodiscard]] Matrix4x4 scaled(const Vector3<T>& scale) const noexcept;
		[[nodiscard]] Matrix4x4 translated(const Vector3<T>& translation) const noexcept;

		[[nodiscard]] Matrix4x4<F> rotated(const Vector3<F>& rotation) const noexcept;
		[[nodiscard]] Matrix4x4<F> rotated(const Quaternion<F>& rotation) const noexcept;

		[[nodiscard]] Vector3<T> get_translation() const noexcept;

		Matrix4x4& operator+=(const Matrix4x4& other);
		Matrix4x4& operator-=(const Matrix4x4& other);
		Matrix4x4 operator+(const Matrix4x4& other) const;
		Matrix4x4 operator-(const Matrix4x4& other) const;
		Matrix4x4 operator*(const Matrix4x4& other) const;

		Vector3<T> operator*(const Vector3<T>& other) const;
		Vector4<T> operator*(const Vector4<T>& other) const;

		Matrix4x4& operator*=(const T& scalar);
		Matrix4x4& operator/=(const T& scalar);
		Matrix4x4 operator*(const T& scalar) const;
		Matrix4x4 operator/(const T& scalar) const;

		static constexpr Matrix4x4 identity();
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
	Matrix4x4<T>::Matrix4x4()
		: elements({})
	{ }

	template <number T>
	Matrix4x4<T>::Matrix4x4(const std::array<T, 16>& elements)
		: elements(elements)
	{ }

	template <number T>
	template <number U>
	requires std::convertible_to<U, T>
	Matrix4x4<T>::Matrix4x4(const Matrix4x4<U>& other)
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			elements[i] = static_cast<T>(other.elements[i]);
		}
	}

	template <number T>
	T& Matrix4x4<T>::get(uint8_t row, uint8_t col)
	{
		return elements[col * 4 + row];
	}

	template <number T>
	const T& Matrix4x4<T>::get(uint8_t row, uint8_t col) const
	{
		return elements[col * 4 + row];
	}

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::transposed() const noexcept
	{
		Matrix4x4 result;
		for (uint8_t row = 0; row < 4; row++)
		{
			for (uint8_t col = 0; col < 4; col++)
			{
				result.get(row, col) = get(col, row);
			}
		}

		return result;
	}

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::scaled(const Vector3<T>& scale) const noexcept
	{
		const Vector3<T>& s = scale;
		const Matrix4x4 m({
			s.x, 0,   0,   0,
			0,   s.y, 0,   0,
			0,   0,   s.z, 0,
			0,   0,   0,   1
			});

		return m * (*this);
	}

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::translated(const Vector3<T>& translation) const noexcept
	{
		const Vector3<T>& t = translation;
		const Matrix4x4 m({
			1,   0,   0,   0,
			0,	 1,   0,   0,
			0,   0,   1,   0,
			t.x, t.y, t.z, 1
		});

		return m * (*this);
	}

	template <number T>
	Matrix4x4<make_floating_t<T>> Matrix4x4<T>::rotated(const Vector3<F>& rotation) const noexcept
	{
		// x = pitch, y = yaw, z = roll
		const Vector3<F> r = rotation * std::numbers::pi_v<T> / 180;
		Matrix4x4<F> pitch = identity();
		Matrix4x4<F> yaw = identity();
		Matrix4x4<F> roll = identity();

		if (rotation.x != 0)
		{
			pitch = Matrix4x4<F>({
				1,   0,		         0,		        0,
				0,   std::cos(r.x),  std::sin(r.x), 0,
				0,   -std::sin(r.x), std::cos(r.x), 0,
				0,   0,		         0,		        1
			});
		}

		if (rotation.y != 0)
		{
			yaw = Matrix4x4<F>({
				std::cos(r.y), 0, -std::sin(r.y), 0,
				0,             1, 0,              0,
				std::sin(r.y), 0, std::cos(r.y),  0,
				0,             0, 0,              1
			});
		}

		if (rotation.z != 0)
		{
			roll = Matrix4x4<F>({
				std::cos(r.z),  std::sin(r.z), 0, 0,
				-std::sin(r.z), std::cos(r.z), 0, 0,
				0,              0,             1, 0,
				0,              0,		       0, 1
			});
		}

		return roll * pitch * yaw * *this;
	}

	template <number T>
	Matrix4x4<typename Matrix4x4<T>::F> Matrix4x4<T>::rotated(const Quaternion<F>& rotation) const noexcept
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
		return Vector3f<T>(elements[12], elements[13], elements[14]);
	}

	template <number T>
	Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4& other)
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			elements[i] += other.elements[i];
		}

		return *this;
	}

	template <number T>
	Matrix4x4<T>& Matrix4x4<T>::operator-=(const Matrix4x4& other)
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			elements[i] -= other.elements[i];
		}

		return *this;
	}

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::operator+(const Matrix4x4& other) const
	{
		Matrix4x4 result = *this;
		return result += other;
	}

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::operator-(const Matrix4x4& other) const
	{
		Matrix4x4 result = *this;
		return result -= other;
	}

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4& other) const
	{
		Matrix4x4 result;
		const Matrix4x4& lhs = *this;
		const Matrix4x4& rhs = other;

		for (uint8_t row = 0; row < 4; row++)
		{
			for (uint8_t col = 0; col < 4; col++)
			{
				T num = 0;
				for (uint8_t i = 0; i < 4; i++)
				{
					num += lhs.get(row, i) * rhs.get(i, col);
				}

				result.get(row, col) = num;
			}
		}

		return result;
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
			other.x * get(0, 0) + other.y * get(0, 1) + other.z * get(0, 2) + other.w * get(0, 3),
			other.x * get(1, 0) + other.y * get(1, 1) + other.z * get(1, 2) + other.w * get(1, 3),
			other.x * get(2, 0) + other.y * get(2, 1) + other.z * get(2, 2) + other.w * get(2, 3),
			other.x * get(3, 0) + other.y * get(3, 1) + other.z * get(3, 2) + other.w * get(3, 3)
		);
	}

	template <number T>
	Matrix4x4<T>& Matrix4x4<T>::operator*=(const T& scalar)
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			elements[i] *= scalar;
		}

		return *this;
	}

	template <number T>
	Matrix4x4<T>& Matrix4x4<T>::operator/=(const T& scalar)
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			elements[i] /= scalar;
		}

		return *this;
	}

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::operator*(const T& scalar) const
	{
		Matrix4x4 result = *this;
		return result *= scalar;
	}

	template <number T>
	Matrix4x4<T> Matrix4x4<T>::operator/(const T& scalar) const
	{
		Matrix4x4 result = *this;
		return result /= scalar;
	}
}
