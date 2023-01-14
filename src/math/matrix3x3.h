#pragma once

#include "matrix.h"
#include "vector3.h"

namespace math
{
	template <number T>
	struct Matrix3x3 : public Matrix<T, 3, 3>
	{
	public:
		Matrix3x3();
		Matrix3x3(const Matrix<T, 3, 3>& m);
		explicit Matrix3x3(const std::array<T, 9>& elements);
		explicit Matrix3x3(const Matrix<T, 4, 4>& m4x4);

		Vector3<T> operator*(const Vector3<T>& other) const;

		static constexpr Matrix3x3 identity();
	};

	using Matrix3x3f = Matrix3x3<float>;
	using Matrix3x3d = Matrix3x3<double>;
	using Matrix3x3i = Matrix3x3<int32_t>;
	using Matrix3x3u = Matrix3x3<uint32_t>;

	template <number T>
	constexpr Matrix3x3<T> Matrix3x3<T>::identity()
	{
		return Matrix3x3({
			1, 0, 0,
			0, 1, 0,
			0, 0, 1,
		});
	}

	template <number T>
	Matrix3x3<T>::Matrix3x3()
		: Matrix<T, 3, 3>()
	{ }

	template <number T>
	Matrix3x3<T>::Matrix3x3(const Matrix<T, 3, 3>& m)
		: Matrix<T, 3, 3>(m)
	{ }

	template <number T>
	Matrix3x3<T>::Matrix3x3(const std::array<T, 9>& elements)
		: Matrix<T, 3, 3>(elements)
	{ }

	template <number T>
	Matrix3x3<T>::Matrix3x3(const Matrix<T, 4, 4>& m4x4)
		: Matrix<T, 3, 3>({
			m4x4.get(0, 0), m4x4.get(1, 0), m4x4.get(2, 0),
			m4x4.get(0, 1), m4x4.get(1, 1), m4x4.get(2, 1),
			m4x4.get(0, 2), m4x4.get(1, 2), m4x4.get(2, 2),
		})
	{ }

	template <number T>
	Vector3<T> Matrix3x3<T>::operator*(const Vector3<T>& other) const
	{
		return Vector3<T>(
			other.x * this->get(0, 0) + other.y * this->get(0, 1) + other.z * this->get(0, 2),
			other.x * this->get(1, 0) + other.y * this->get(1, 1) + other.z * this->get(1, 2),
			other.x * this->get(2, 0) + other.y * this->get(2, 1) + other.z * this->get(2, 2)
		);
	}
}
