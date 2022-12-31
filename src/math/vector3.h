#pragma once

#include "vector2.h"

namespace math
{
	template <number T>
	struct Vector3
	{
	public:
		using F = make_floating_t<T>;

		T x;
		T y;
		T z;

		Vector3();
		Vector3(T x, T y, T z);

		template <number U>
		requires std::convertible_to<U, T>
		Vector3(const Vector3<U>& other);

#pragma region Swizzling

		Vector3(T x, const Vector2<T>& yz) : x(x), y(yz.x), z(yz.y) { }
		Vector3(const Vector2<T>& xy, T z) : x(xy.x), y(xy.y), z(z) { }

		[[nodiscard]] Vector2<T> xx() const noexcept { return Vector2<T>(x, x); }
		[[nodiscard]] Vector2<T> xy() const noexcept { return Vector2<T>(x, y); }
		[[nodiscard]] Vector2<T> xz() const noexcept { return Vector2<T>(x, z); }
		[[nodiscard]] Vector2<T> yx() const noexcept { return Vector2<T>(y, x); }
		[[nodiscard]] Vector2<T> yy() const noexcept { return Vector2<T>(y, y); }
		[[nodiscard]] Vector2<T> yz() const noexcept { return Vector2<T>(y, z); }
		[[nodiscard]] Vector2<T> zx() const noexcept { return Vector2<T>(z, x); }
		[[nodiscard]] Vector2<T> zy() const noexcept { return Vector2<T>(z, y); }
		[[nodiscard]] Vector2<T> zz() const noexcept { return Vector2<T>(z, z); }

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

		[[nodiscard]] T volume() const noexcept;
		[[nodiscard]] T min() const noexcept;
		[[nodiscard]] T max() const noexcept;
		[[nodiscard]] T magnitude_sqr() const noexcept;
		[[nodiscard]] F magnitude() const noexcept;
		[[nodiscard]] Vector3<F> normalized() const noexcept;
		[[nodiscard]] Vector3<F> normalized_unsafe() const noexcept;
		[[nodiscard]] Vector3<F> reciprocal() const noexcept;

		Vector3 operator+() const;
		Vector3 operator-() const;

		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator*=(const Vector3& other);
		Vector3& operator/=(const Vector3& other);
		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const Vector3& other) const;
		Vector3 operator/(const Vector3& other) const;

		Vector3& operator*=(const T& scalar);
		Vector3& operator/=(const T& scalar);
		Vector3 operator*(const T& scalar) const;
		Vector3 operator/(const T& scalar) const;

		static constexpr Vector3 zero() { return Vector3(0, 0, 0); }
		static constexpr Vector3 one() { return Vector3(1, 1, 1); }
	};

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3i = Vector3<int32_t>;
	using Vector3u = Vector3<uint32_t>;
	using Vector3u8 = Vector3<uint8_t>;

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
	template <number U>
	requires std::convertible_to<U, T>
	Vector3<T>::Vector3(const Vector3<U>& other)
		: x(static_cast<T>(other.x))
		, y(static_cast<T>(other.y))
		, z(static_cast<T>(other.z))
	{ }

	template <number T>
	T Vector3<T>::volume() const noexcept
	{
		return x * y * z;
	}

	template <number T>
	T Vector3<T>::min() const noexcept
	{
		return std::min(x, y, z);
	}

	template <number T>
	T Vector3<T>::max() const noexcept
	{
		return std::max(x, y, z);
	}

	template <number T>
	T Vector3<T>::magnitude_sqr() const noexcept
	{
		return x * x + y * y + z * z;
	}

	template <number T>
	make_floating_t<T> Vector3<T>::magnitude() const noexcept
	{
		return std::sqrt(static_cast<F>(magnitude_sqr()));
	}

	template <number T>
	Vector3<make_floating_t<T>> Vector3<T>::normalized() const noexcept
	{
		const F mag = magnitude();
		if (mag != 0)
		{
			return static_cast<Vector3<F>>(*this) / mag;
		}

		return *this;
	}

	template <number T>
	Vector3<make_floating_t<T>> Vector3<T>::normalized_unsafe() const noexcept
	{
		return static_cast<Vector3<F>>(*this) / magnitude();
	}

	template <number T>
	Vector3<make_floating_t<T>> Vector3<T>::reciprocal() const noexcept
	{
		constexpr F one_f = 1;
		return Vector3<F>(one_f / x, one_f / y, one_f / z);
	}

	template <number T>
	Vector3<T> Vector3<T>::operator+() const
	{
		return *this;
	}

	template <number T>
	Vector3<T> Vector3<T>::operator-() const
	{
		return *this * -1;
	}

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
	Vector3<T>& Vector3<T>::operator*=(const Vector3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	template <number T>
	Vector3<T>& Vector3<T>::operator/=(const Vector3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}

	template <number T>
	Vector3<T> Vector3<T>::operator+(const Vector3& other) const
	{
		Vector3 result = *this;
		return result += other;
	}

	template <number T>
	Vector3<T> Vector3<T>::operator-(const Vector3& other) const
	{
		Vector3 result = *this;
		return result -= other;
	}

	template <number T>
	Vector3<T> Vector3<T>::operator*(const Vector3& other) const
	{
		Vector3 result = *this;
		return result *= other;
	}

	template <number T>
	Vector3<T> Vector3<T>::operator/(const Vector3& other) const
	{
		Vector3 result = *this;
		return result /= other;
	}

	template <number T>
	Vector3<T>& Vector3<T>::operator*=(const T& scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	template <number T>
	Vector3<T>& Vector3<T>::operator/=(const T& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	template <number T>
	Vector3<T> Vector3<T>::operator*(const T& scalar) const
	{
		Vector3 result = *this;
		return result *= scalar;
	}

	template <number T>
	Vector3<T> Vector3<T>::operator/(const T& scalar) const
	{
		Vector3 result = *this;
		return result /= scalar;
	}
}
