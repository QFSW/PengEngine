#pragma once

#include <array>

#include "number.h"

namespace math
{
	template <number T, uint8_t Rows, uint8_t Cols>
	struct Matrix
	{
	public:
		static constexpr size_t num = Rows * Cols;

		std::array<T, num> elements;

		Matrix();
		explicit Matrix(const std::array<T, num>& elements);

		template <number U>
		requires std::convertible_to<U, T>
		Matrix(const Matrix<U, Rows, Cols>& other);

		[[nodiscard]] T& get(uint8_t row, uint8_t col);
		[[nodiscard]] const T& get(uint8_t row, uint8_t col) const;

		[[nodiscard]] Matrix transposed() const noexcept;

		[[nodiscard]] Matrix inverse() const noexcept;
		[[nodiscard]] Matrix adjugate() const noexcept;
		[[nodiscard]] T trace() const noexcept;
		[[nodiscard]] T determinant() const noexcept;
		[[nodiscard]] T cofactor(uint8_t row, uint8_t col) const noexcept;
		[[nodiscard]] T minor(uint8_t row, uint8_t col) const noexcept;
		[[nodiscard]] Matrix<T, Rows - 1, Cols - 1> minor_matrix(uint8_t row, uint8_t col) const noexcept;

		Matrix& operator+=(const Matrix& other);
		Matrix& operator-=(const Matrix& other);
		Matrix operator+(const Matrix& other) const;
		Matrix operator-(const Matrix& other) const;

		Matrix& operator*=(const T& scalar);
		Matrix& operator/=(const T& scalar);
		Matrix operator*(const T& scalar) const;
		Matrix operator/(const T& scalar) const;
	};

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols>::Matrix()
		: elements({})
	{ }

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols>::Matrix(const std::array<T, num>& elements)
		: elements(elements)
	{ }

	template <number T, uint8_t Rows, uint8_t Cols>
	template <number U>
	requires std::convertible_to<U, T>
	Matrix<T, Rows, Cols>::Matrix(const Matrix<U, Rows, Cols>& other)
	{
		for (uint8_t i = 0; i < num; i++)
		{
			elements[i] = static_cast<T>(other.elements[i]);
		}
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	T& Matrix<T, Rows, Cols>::get(uint8_t row, uint8_t col)
	{
		return elements[col * Rows + row];
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	const T& Matrix<T, Rows, Cols>::get(uint8_t row, uint8_t col) const
	{
		return elements[col * Rows + row];
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::transposed() const noexcept
	{
		Matrix result;
		for (uint8_t row = 0; row < Rows; row++)
		{
			for (uint8_t col = 0; col < Cols; col++)
			{
				result.get(row, col) = get(col, row);
			}
		}

		return result;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::inverse() const noexcept
	{
		const T det = determinant();
		const T inv_det = 1 / det;

		if constexpr (Rows == 2)
		{
			return Matrix({
				+get(1, 1), -get(1, 0),
				-get(0, 1), +get(0, 0)
			}) * inv_det;
		}
		else
		{
			return adjugate() * inv_det;
		}
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::adjugate() const noexcept
	{
		Matrix result;
		for (uint8_t row = 0; row < Rows; row++)
		{
			for (uint8_t col = 0; col < Cols; col++)
			{
				result.get(row, col) = cofactor(row, col) * minor(col, row);
			}
		}

		return result;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	T Matrix<T, Rows, Cols>::trace() const noexcept
	{
		T result = 0;
		for (uint8_t i = 0; i < Rows; i++)
		{
			result += get(i, i);
		}

		return result;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	T Matrix<T, Rows, Cols>::determinant() const noexcept
	{
		static_assert(Rows == Cols, "Matrix must be square to calculate determinant");

		if constexpr (Rows == 1)
		{
			return elements[0];
		}
		if constexpr (Rows == 2)
		{
			return elements[0] * elements[3] - elements[1] * elements[2];
		}
		else
		{
			T result = 0;
			for (uint8_t col = 0; col < Cols; col++)
			{
				result += cofactor(0, col) * get(0, col) * minor(0, col);
			}

			return result;
		}
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	T Matrix<T, Rows, Cols>::cofactor(uint8_t row, uint8_t col) const noexcept
	{
		const uint8_t n = row + col;
		return n % 2 == 0
			? static_cast<T>(+1)
			: static_cast<T>(-1);
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	T Matrix<T, Rows, Cols>::minor(uint8_t row, uint8_t col) const noexcept
	{
		return minor_matrix(row, col).determinant();
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows - 1, Cols - 1> Matrix<T, Rows, Cols>::minor_matrix(uint8_t row, uint8_t col) const noexcept
	{
		Matrix<T, Rows - 1, Cols - 1> result;

		uint8_t write_row = 0;
		for (uint8_t read_row = 0; read_row < Rows; read_row++)
		{
			if (read_row != row)
			{
				uint8_t write_col = 0;
				for (uint8_t read_col = 0; read_col < Cols; read_col++)
				{
					if (read_col != col)
					{
						result.get(write_row, write_col) = get(read_row, read_col);
						write_col++;
					}
				}

				write_row++;
			}
		}

		return result;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols>& Matrix<T, Rows, Cols>::operator+=(const Matrix& other)
	{
		for (uint8_t i = 0; i < num; i++)
		{
			elements[i] += other.elements[i];
		}

		return *this;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols>& Matrix<T, Rows, Cols>::operator-=(const Matrix& other)
	{
		for (uint8_t i = 0; i < num; i++)
		{
			elements[i] -= other.elements[i];
		}

		return *this;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator+(const Matrix& other) const
	{
		Matrix result = *this;
		return result += other;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator-(const Matrix& other) const
	{
		Matrix result = *this;
		return result -= other;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols>& Matrix<T, Rows, Cols>::operator*=(const T& scalar)
	{
		for (uint8_t i = 0; i < num; i++)
		{
			elements[i] *= scalar;
		}

		return *this;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols>& Matrix<T, Rows, Cols>::operator/=(const T& scalar)
	{
		for (uint8_t i = 0; i < num; i++)
		{
			elements[i] /= scalar;
		}

		return *this;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator*(const T& scalar) const
	{
		Matrix result = *this;
		return result *= scalar;
	}

	template <number T, uint8_t Rows, uint8_t Cols>
	Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator/(const T& scalar) const
	{
		Matrix result = *this;
		return result /= scalar;
	}

	template <number T, uint8_t Rows, uint8_t Cols, uint8_t ColsB>
	Matrix<T, Rows, ColsB> operator*(const Matrix<T, Rows, Cols>& lhs, const Matrix<T, Cols, ColsB>& rhs)
	{
		Matrix<T, Rows, ColsB> result;

		for (uint8_t row = 0; row < Rows; row++)
		{
			for (uint8_t col = 0; col < ColsB; col++)
			{
				T num = 0;
				for (uint8_t i = 0; i < Cols; i++)
				{
					num += lhs.get(row, i) * rhs.get(i, col);
				}

				result.get(row, col) = num;
			}
		}

		return result;
	}
}
