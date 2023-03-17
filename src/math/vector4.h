#pragma once

#include "vector3.h"

namespace math
{
	template <number T>
	struct Vector4
	{
	public:
		using F = make_floating_t<T>;

		T x;
		T y;
		T z;
		T w;

		Vector4();
		Vector4(T x, T y, T z, T w);

		template <number U>
		requires std::convertible_to<U, T>
		Vector4(const Vector4<U>& other);

#pragma region Swizzling

		Vector4(T x, const Vector3<T>& yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) { }
		Vector4(const Vector3<T>& xyz, T w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }

		Vector4(T x, T y, const Vector2<T>& zw) : x(x), y(y), z(zw.x), w(zw.y) { }
		Vector4(T x, const Vector2<T>& yz, T w) : x(x), y(yz.x), z(yz.y), w(w) { }
		Vector4(const Vector2<T>& xy, T z, T w) : x(xy.x), y(xy.y), z(z), w(w) { }

		Vector4(const Vector2<T>& xy, const Vector2<T>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }

		[[nodiscard]] Vector2<T> xx() const noexcept { return Vector2<T>(x, x); }
		[[nodiscard]] Vector2<T> xy() const noexcept { return Vector2<T>(x, y); }
		[[nodiscard]] Vector2<T> xz() const noexcept { return Vector2<T>(x, z); }
		[[nodiscard]] Vector2<T> xw() const noexcept { return Vector2<T>(x, w); }
		[[nodiscard]] Vector2<T> yx() const noexcept { return Vector2<T>(y, x); }
		[[nodiscard]] Vector2<T> yy() const noexcept { return Vector2<T>(y, y); }
		[[nodiscard]] Vector2<T> yz() const noexcept { return Vector2<T>(y, z); }
		[[nodiscard]] Vector2<T> yw() const noexcept { return Vector2<T>(y, w); }
		[[nodiscard]] Vector2<T> zx() const noexcept { return Vector2<T>(z, x); }
		[[nodiscard]] Vector2<T> zy() const noexcept { return Vector2<T>(z, y); }
		[[nodiscard]] Vector2<T> zz() const noexcept { return Vector2<T>(z, z); }
		[[nodiscard]] Vector2<T> zw() const noexcept { return Vector2<T>(z, w); }
		[[nodiscard]] Vector2<T> wx() const noexcept { return Vector2<T>(w, x); }
		[[nodiscard]] Vector2<T> wy() const noexcept { return Vector2<T>(w, y); }
		[[nodiscard]] Vector2<T> wz() const noexcept { return Vector2<T>(w, z); }
		[[nodiscard]] Vector2<T> ww() const noexcept { return Vector2<T>(w, w); }

		[[nodiscard]] Vector3<T> xxx() const noexcept { return Vector3<T>(x, x, x); }
		[[nodiscard]] Vector3<T> xxy() const noexcept { return Vector3<T>(x, x, y); }
		[[nodiscard]] Vector3<T> xxz() const noexcept { return Vector3<T>(x, x, z); }
		[[nodiscard]] Vector3<T> xxw() const noexcept { return Vector3<T>(x, x, w); }
		[[nodiscard]] Vector3<T> xyx() const noexcept { return Vector3<T>(x, y, x); }
		[[nodiscard]] Vector3<T> xyy() const noexcept { return Vector3<T>(x, y, y); }
		[[nodiscard]] Vector3<T> xyz() const noexcept { return Vector3<T>(x, y, z); }
		[[nodiscard]] Vector3<T> xyw() const noexcept { return Vector3<T>(x, y, w); }
		[[nodiscard]] Vector3<T> xzx() const noexcept { return Vector3<T>(x, z, x); }
		[[nodiscard]] Vector3<T> xzy() const noexcept { return Vector3<T>(x, z, y); }
		[[nodiscard]] Vector3<T> xzz() const noexcept { return Vector3<T>(x, z, z); }
		[[nodiscard]] Vector3<T> xzw() const noexcept { return Vector3<T>(x, z, w); }
		[[nodiscard]] Vector3<T> xwx() const noexcept { return Vector3<T>(x, w, x); }
		[[nodiscard]] Vector3<T> xwy() const noexcept { return Vector3<T>(x, w, y); }
		[[nodiscard]] Vector3<T> xwz() const noexcept { return Vector3<T>(x, w, z); }
		[[nodiscard]] Vector3<T> xww() const noexcept { return Vector3<T>(x, w, w); }
		[[nodiscard]] Vector3<T> yxx() const noexcept { return Vector3<T>(y, x, x); }
		[[nodiscard]] Vector3<T> yxy() const noexcept { return Vector3<T>(y, x, y); }
		[[nodiscard]] Vector3<T> yxz() const noexcept { return Vector3<T>(y, x, z); }
		[[nodiscard]] Vector3<T> yxw() const noexcept { return Vector3<T>(y, x, w); }
		[[nodiscard]] Vector3<T> yyx() const noexcept { return Vector3<T>(y, y, x); }
		[[nodiscard]] Vector3<T> yyy() const noexcept { return Vector3<T>(y, y, y); }
		[[nodiscard]] Vector3<T> yyz() const noexcept { return Vector3<T>(y, y, z); }
		[[nodiscard]] Vector3<T> yyw() const noexcept { return Vector3<T>(y, y, w); }
		[[nodiscard]] Vector3<T> yzx() const noexcept { return Vector3<T>(y, z, x); }
		[[nodiscard]] Vector3<T> yzy() const noexcept { return Vector3<T>(y, z, y); }
		[[nodiscard]] Vector3<T> yzz() const noexcept { return Vector3<T>(y, z, z); }
		[[nodiscard]] Vector3<T> yzw() const noexcept { return Vector3<T>(y, z, w); }
		[[nodiscard]] Vector3<T> ywx() const noexcept { return Vector3<T>(y, w, x); }
		[[nodiscard]] Vector3<T> ywy() const noexcept { return Vector3<T>(y, w, y); }
		[[nodiscard]] Vector3<T> ywz() const noexcept { return Vector3<T>(y, w, z); }
		[[nodiscard]] Vector3<T> yww() const noexcept { return Vector3<T>(y, w, w); }
		[[nodiscard]] Vector3<T> zxx() const noexcept { return Vector3<T>(z, x, x); }
		[[nodiscard]] Vector3<T> zxy() const noexcept { return Vector3<T>(z, x, y); }
		[[nodiscard]] Vector3<T> zxz() const noexcept { return Vector3<T>(z, x, z); }
		[[nodiscard]] Vector3<T> zxw() const noexcept { return Vector3<T>(z, x, w); }
		[[nodiscard]] Vector3<T> zyx() const noexcept { return Vector3<T>(z, y, x); }
		[[nodiscard]] Vector3<T> zyy() const noexcept { return Vector3<T>(z, y, y); }
		[[nodiscard]] Vector3<T> zyz() const noexcept { return Vector3<T>(z, y, z); }
		[[nodiscard]] Vector3<T> zyw() const noexcept { return Vector3<T>(z, y, w); }
		[[nodiscard]] Vector3<T> zzx() const noexcept { return Vector3<T>(z, z, x); }
		[[nodiscard]] Vector3<T> zzy() const noexcept { return Vector3<T>(z, z, y); }
		[[nodiscard]] Vector3<T> zzz() const noexcept { return Vector3<T>(z, z, z); }
		[[nodiscard]] Vector3<T> zzw() const noexcept { return Vector3<T>(z, z, w); }
		[[nodiscard]] Vector3<T> zwx() const noexcept { return Vector3<T>(z, w, x); }
		[[nodiscard]] Vector3<T> zwy() const noexcept { return Vector3<T>(z, w, y); }
		[[nodiscard]] Vector3<T> zwz() const noexcept { return Vector3<T>(z, w, z); }
		[[nodiscard]] Vector3<T> zww() const noexcept { return Vector3<T>(z, w, w); }
		[[nodiscard]] Vector3<T> wxx() const noexcept { return Vector3<T>(w, x, x); }
		[[nodiscard]] Vector3<T> wxy() const noexcept { return Vector3<T>(w, x, y); }
		[[nodiscard]] Vector3<T> wxz() const noexcept { return Vector3<T>(w, x, z); }
		[[nodiscard]] Vector3<T> wxw() const noexcept { return Vector3<T>(w, x, w); }
		[[nodiscard]] Vector3<T> wyx() const noexcept { return Vector3<T>(w, y, x); }
		[[nodiscard]] Vector3<T> wyy() const noexcept { return Vector3<T>(w, y, y); }
		[[nodiscard]] Vector3<T> wyz() const noexcept { return Vector3<T>(w, y, z); }
		[[nodiscard]] Vector3<T> wyw() const noexcept { return Vector3<T>(w, y, w); }
		[[nodiscard]] Vector3<T> wzx() const noexcept { return Vector3<T>(w, z, x); }
		[[nodiscard]] Vector3<T> wzy() const noexcept { return Vector3<T>(w, z, y); }
		[[nodiscard]] Vector3<T> wzz() const noexcept { return Vector3<T>(w, z, z); }
		[[nodiscard]] Vector3<T> wzw() const noexcept { return Vector3<T>(w, z, w); }
		[[nodiscard]] Vector3<T> wwx() const noexcept { return Vector3<T>(w, w, x); }
		[[nodiscard]] Vector3<T> wwy() const noexcept { return Vector3<T>(w, w, y); }
		[[nodiscard]] Vector3<T> wwz() const noexcept { return Vector3<T>(w, w, z); }
		[[nodiscard]] Vector3<T> www() const noexcept { return Vector3<T>(w, w, w); }

		[[nodiscard]] Vector4 xxxx() const noexcept { return Vector4<T>(x, x, x, x); }
		[[nodiscard]] Vector4 xxxy() const noexcept { return Vector4<T>(x, x, x, y); }
		[[nodiscard]] Vector4 xxxz() const noexcept { return Vector4<T>(x, x, x, z); }
		[[nodiscard]] Vector4 xxxw() const noexcept { return Vector4<T>(x, x, x, w); }
		[[nodiscard]] Vector4 xxyx() const noexcept { return Vector4<T>(x, x, y, x); }
		[[nodiscard]] Vector4 xxyy() const noexcept { return Vector4<T>(x, x, y, y); }
		[[nodiscard]] Vector4 xxyz() const noexcept { return Vector4<T>(x, x, y, z); }
		[[nodiscard]] Vector4 xxyw() const noexcept { return Vector4<T>(x, x, y, w); }
		[[nodiscard]] Vector4 xxzx() const noexcept { return Vector4<T>(x, x, z, x); }
		[[nodiscard]] Vector4 xxzy() const noexcept { return Vector4<T>(x, x, z, y); }
		[[nodiscard]] Vector4 xxzz() const noexcept { return Vector4<T>(x, x, z, z); }
		[[nodiscard]] Vector4 xxzw() const noexcept { return Vector4<T>(x, x, z, w); }
		[[nodiscard]] Vector4 xxwx() const noexcept { return Vector4<T>(x, x, w, x); }
		[[nodiscard]] Vector4 xxwy() const noexcept { return Vector4<T>(x, x, w, y); }
		[[nodiscard]] Vector4 xxwz() const noexcept { return Vector4<T>(x, x, w, z); }
		[[nodiscard]] Vector4 xxww() const noexcept { return Vector4<T>(x, x, w, w); }
		[[nodiscard]] Vector4 xyxx() const noexcept { return Vector4<T>(x, y, x, x); }
		[[nodiscard]] Vector4 xyxy() const noexcept { return Vector4<T>(x, y, x, y); }
		[[nodiscard]] Vector4 xyxz() const noexcept { return Vector4<T>(x, y, x, z); }
		[[nodiscard]] Vector4 xyxw() const noexcept { return Vector4<T>(x, y, x, w); }
		[[nodiscard]] Vector4 xyyx() const noexcept { return Vector4<T>(x, y, y, x); }
		[[nodiscard]] Vector4 xyyy() const noexcept { return Vector4<T>(x, y, y, y); }
		[[nodiscard]] Vector4 xyyz() const noexcept { return Vector4<T>(x, y, y, z); }
		[[nodiscard]] Vector4 xyyw() const noexcept { return Vector4<T>(x, y, y, w); }
		[[nodiscard]] Vector4 xyzx() const noexcept { return Vector4<T>(x, y, z, x); }
		[[nodiscard]] Vector4 xyzy() const noexcept { return Vector4<T>(x, y, z, y); }
		[[nodiscard]] Vector4 xyzz() const noexcept { return Vector4<T>(x, y, z, z); }
		[[nodiscard]] Vector4 xyzw() const noexcept { return Vector4<T>(x, y, z, w); }
		[[nodiscard]] Vector4 xywx() const noexcept { return Vector4<T>(x, y, w, x); }
		[[nodiscard]] Vector4 xywy() const noexcept { return Vector4<T>(x, y, w, y); }
		[[nodiscard]] Vector4 xywz() const noexcept { return Vector4<T>(x, y, w, z); }
		[[nodiscard]] Vector4 xyww() const noexcept { return Vector4<T>(x, y, w, w); }
		[[nodiscard]] Vector4 xzxx() const noexcept { return Vector4<T>(x, z, x, x); }
		[[nodiscard]] Vector4 xzxy() const noexcept { return Vector4<T>(x, z, x, y); }
		[[nodiscard]] Vector4 xzxz() const noexcept { return Vector4<T>(x, z, x, z); }
		[[nodiscard]] Vector4 xzxw() const noexcept { return Vector4<T>(x, z, x, w); }
		[[nodiscard]] Vector4 xzyx() const noexcept { return Vector4<T>(x, z, y, x); }
		[[nodiscard]] Vector4 xzyy() const noexcept { return Vector4<T>(x, z, y, y); }
		[[nodiscard]] Vector4 xzyz() const noexcept { return Vector4<T>(x, z, y, z); }
		[[nodiscard]] Vector4 xzyw() const noexcept { return Vector4<T>(x, z, y, w); }
		[[nodiscard]] Vector4 xzzx() const noexcept { return Vector4<T>(x, z, z, x); }
		[[nodiscard]] Vector4 xzzy() const noexcept { return Vector4<T>(x, z, z, y); }
		[[nodiscard]] Vector4 xzzz() const noexcept { return Vector4<T>(x, z, z, z); }
		[[nodiscard]] Vector4 xzzw() const noexcept { return Vector4<T>(x, z, z, w); }
		[[nodiscard]] Vector4 xzwx() const noexcept { return Vector4<T>(x, z, w, x); }
		[[nodiscard]] Vector4 xzwy() const noexcept { return Vector4<T>(x, z, w, y); }
		[[nodiscard]] Vector4 xzwz() const noexcept { return Vector4<T>(x, z, w, z); }
		[[nodiscard]] Vector4 xzww() const noexcept { return Vector4<T>(x, z, w, w); }
		[[nodiscard]] Vector4 xwxx() const noexcept { return Vector4<T>(x, w, x, x); }
		[[nodiscard]] Vector4 xwxy() const noexcept { return Vector4<T>(x, w, x, y); }
		[[nodiscard]] Vector4 xwxz() const noexcept { return Vector4<T>(x, w, x, z); }
		[[nodiscard]] Vector4 xwxw() const noexcept { return Vector4<T>(x, w, x, w); }
		[[nodiscard]] Vector4 xwyx() const noexcept { return Vector4<T>(x, w, y, x); }
		[[nodiscard]] Vector4 xwyy() const noexcept { return Vector4<T>(x, w, y, y); }
		[[nodiscard]] Vector4 xwyz() const noexcept { return Vector4<T>(x, w, y, z); }
		[[nodiscard]] Vector4 xwyw() const noexcept { return Vector4<T>(x, w, y, w); }
		[[nodiscard]] Vector4 xwzx() const noexcept { return Vector4<T>(x, w, z, x); }
		[[nodiscard]] Vector4 xwzy() const noexcept { return Vector4<T>(x, w, z, y); }
		[[nodiscard]] Vector4 xwzz() const noexcept { return Vector4<T>(x, w, z, z); }
		[[nodiscard]] Vector4 xwzw() const noexcept { return Vector4<T>(x, w, z, w); }
		[[nodiscard]] Vector4 xwwx() const noexcept { return Vector4<T>(x, w, w, x); }
		[[nodiscard]] Vector4 xwwy() const noexcept { return Vector4<T>(x, w, w, y); }
		[[nodiscard]] Vector4 xwwz() const noexcept { return Vector4<T>(x, w, w, z); }
		[[nodiscard]] Vector4 xwww() const noexcept { return Vector4<T>(x, w, w, w); }
		[[nodiscard]] Vector4 yxxx() const noexcept { return Vector4<T>(y, x, x, x); }
		[[nodiscard]] Vector4 yxxy() const noexcept { return Vector4<T>(y, x, x, y); }
		[[nodiscard]] Vector4 yxxz() const noexcept { return Vector4<T>(y, x, x, z); }
		[[nodiscard]] Vector4 yxxw() const noexcept { return Vector4<T>(y, x, x, w); }
		[[nodiscard]] Vector4 yxyx() const noexcept { return Vector4<T>(y, x, y, x); }
		[[nodiscard]] Vector4 yxyy() const noexcept { return Vector4<T>(y, x, y, y); }
		[[nodiscard]] Vector4 yxyz() const noexcept { return Vector4<T>(y, x, y, z); }
		[[nodiscard]] Vector4 yxyw() const noexcept { return Vector4<T>(y, x, y, w); }
		[[nodiscard]] Vector4 yxzx() const noexcept { return Vector4<T>(y, x, z, x); }
		[[nodiscard]] Vector4 yxzy() const noexcept { return Vector4<T>(y, x, z, y); }
		[[nodiscard]] Vector4 yxzz() const noexcept { return Vector4<T>(y, x, z, z); }
		[[nodiscard]] Vector4 yxzw() const noexcept { return Vector4<T>(y, x, z, w); }
		[[nodiscard]] Vector4 yxwx() const noexcept { return Vector4<T>(y, x, w, x); }
		[[nodiscard]] Vector4 yxwy() const noexcept { return Vector4<T>(y, x, w, y); }
		[[nodiscard]] Vector4 yxwz() const noexcept { return Vector4<T>(y, x, w, z); }
		[[nodiscard]] Vector4 yxww() const noexcept { return Vector4<T>(y, x, w, w); }
		[[nodiscard]] Vector4 yyxx() const noexcept { return Vector4<T>(y, y, x, x); }
		[[nodiscard]] Vector4 yyxy() const noexcept { return Vector4<T>(y, y, x, y); }
		[[nodiscard]] Vector4 yyxz() const noexcept { return Vector4<T>(y, y, x, z); }
		[[nodiscard]] Vector4 yyxw() const noexcept { return Vector4<T>(y, y, x, w); }
		[[nodiscard]] Vector4 yyyx() const noexcept { return Vector4<T>(y, y, y, x); }
		[[nodiscard]] Vector4 yyyy() const noexcept { return Vector4<T>(y, y, y, y); }
		[[nodiscard]] Vector4 yyyz() const noexcept { return Vector4<T>(y, y, y, z); }
		[[nodiscard]] Vector4 yyyw() const noexcept { return Vector4<T>(y, y, y, w); }
		[[nodiscard]] Vector4 yyzx() const noexcept { return Vector4<T>(y, y, z, x); }
		[[nodiscard]] Vector4 yyzy() const noexcept { return Vector4<T>(y, y, z, y); }
		[[nodiscard]] Vector4 yyzz() const noexcept { return Vector4<T>(y, y, z, z); }
		[[nodiscard]] Vector4 yyzw() const noexcept { return Vector4<T>(y, y, z, w); }
		[[nodiscard]] Vector4 yywx() const noexcept { return Vector4<T>(y, y, w, x); }
		[[nodiscard]] Vector4 yywy() const noexcept { return Vector4<T>(y, y, w, y); }
		[[nodiscard]] Vector4 yywz() const noexcept { return Vector4<T>(y, y, w, z); }
		[[nodiscard]] Vector4 yyww() const noexcept { return Vector4<T>(y, y, w, w); }
		[[nodiscard]] Vector4 yzxx() const noexcept { return Vector4<T>(y, z, x, x); }
		[[nodiscard]] Vector4 yzxy() const noexcept { return Vector4<T>(y, z, x, y); }
		[[nodiscard]] Vector4 yzxz() const noexcept { return Vector4<T>(y, z, x, z); }
		[[nodiscard]] Vector4 yzxw() const noexcept { return Vector4<T>(y, z, x, w); }
		[[nodiscard]] Vector4 yzyx() const noexcept { return Vector4<T>(y, z, y, x); }
		[[nodiscard]] Vector4 yzyy() const noexcept { return Vector4<T>(y, z, y, y); }
		[[nodiscard]] Vector4 yzyz() const noexcept { return Vector4<T>(y, z, y, z); }
		[[nodiscard]] Vector4 yzyw() const noexcept { return Vector4<T>(y, z, y, w); }
		[[nodiscard]] Vector4 yzzx() const noexcept { return Vector4<T>(y, z, z, x); }
		[[nodiscard]] Vector4 yzzy() const noexcept { return Vector4<T>(y, z, z, y); }
		[[nodiscard]] Vector4 yzzz() const noexcept { return Vector4<T>(y, z, z, z); }
		[[nodiscard]] Vector4 yzzw() const noexcept { return Vector4<T>(y, z, z, w); }
		[[nodiscard]] Vector4 yzwx() const noexcept { return Vector4<T>(y, z, w, x); }
		[[nodiscard]] Vector4 yzwy() const noexcept { return Vector4<T>(y, z, w, y); }
		[[nodiscard]] Vector4 yzwz() const noexcept { return Vector4<T>(y, z, w, z); }
		[[nodiscard]] Vector4 yzww() const noexcept { return Vector4<T>(y, z, w, w); }
		[[nodiscard]] Vector4 ywxx() const noexcept { return Vector4<T>(y, w, x, x); }
		[[nodiscard]] Vector4 ywxy() const noexcept { return Vector4<T>(y, w, x, y); }
		[[nodiscard]] Vector4 ywxz() const noexcept { return Vector4<T>(y, w, x, z); }
		[[nodiscard]] Vector4 ywxw() const noexcept { return Vector4<T>(y, w, x, w); }
		[[nodiscard]] Vector4 ywyx() const noexcept { return Vector4<T>(y, w, y, x); }
		[[nodiscard]] Vector4 ywyy() const noexcept { return Vector4<T>(y, w, y, y); }
		[[nodiscard]] Vector4 ywyz() const noexcept { return Vector4<T>(y, w, y, z); }
		[[nodiscard]] Vector4 ywyw() const noexcept { return Vector4<T>(y, w, y, w); }
		[[nodiscard]] Vector4 ywzx() const noexcept { return Vector4<T>(y, w, z, x); }
		[[nodiscard]] Vector4 ywzy() const noexcept { return Vector4<T>(y, w, z, y); }
		[[nodiscard]] Vector4 ywzz() const noexcept { return Vector4<T>(y, w, z, z); }
		[[nodiscard]] Vector4 ywzw() const noexcept { return Vector4<T>(y, w, z, w); }
		[[nodiscard]] Vector4 ywwx() const noexcept { return Vector4<T>(y, w, w, x); }
		[[nodiscard]] Vector4 ywwy() const noexcept { return Vector4<T>(y, w, w, y); }
		[[nodiscard]] Vector4 ywwz() const noexcept { return Vector4<T>(y, w, w, z); }
		[[nodiscard]] Vector4 ywww() const noexcept { return Vector4<T>(y, w, w, w); }
		[[nodiscard]] Vector4 zxxx() const noexcept { return Vector4<T>(z, x, x, x); }
		[[nodiscard]] Vector4 zxxy() const noexcept { return Vector4<T>(z, x, x, y); }
		[[nodiscard]] Vector4 zxxz() const noexcept { return Vector4<T>(z, x, x, z); }
		[[nodiscard]] Vector4 zxxw() const noexcept { return Vector4<T>(z, x, x, w); }
		[[nodiscard]] Vector4 zxyx() const noexcept { return Vector4<T>(z, x, y, x); }
		[[nodiscard]] Vector4 zxyy() const noexcept { return Vector4<T>(z, x, y, y); }
		[[nodiscard]] Vector4 zxyz() const noexcept { return Vector4<T>(z, x, y, z); }
		[[nodiscard]] Vector4 zxyw() const noexcept { return Vector4<T>(z, x, y, w); }
		[[nodiscard]] Vector4 zxzx() const noexcept { return Vector4<T>(z, x, z, x); }
		[[nodiscard]] Vector4 zxzy() const noexcept { return Vector4<T>(z, x, z, y); }
		[[nodiscard]] Vector4 zxzz() const noexcept { return Vector4<T>(z, x, z, z); }
		[[nodiscard]] Vector4 zxzw() const noexcept { return Vector4<T>(z, x, z, w); }
		[[nodiscard]] Vector4 zxwx() const noexcept { return Vector4<T>(z, x, w, x); }
		[[nodiscard]] Vector4 zxwy() const noexcept { return Vector4<T>(z, x, w, y); }
		[[nodiscard]] Vector4 zxwz() const noexcept { return Vector4<T>(z, x, w, z); }
		[[nodiscard]] Vector4 zxww() const noexcept { return Vector4<T>(z, x, w, w); }
		[[nodiscard]] Vector4 zyxx() const noexcept { return Vector4<T>(z, y, x, x); }
		[[nodiscard]] Vector4 zyxy() const noexcept { return Vector4<T>(z, y, x, y); }
		[[nodiscard]] Vector4 zyxz() const noexcept { return Vector4<T>(z, y, x, z); }
		[[nodiscard]] Vector4 zyxw() const noexcept { return Vector4<T>(z, y, x, w); }
		[[nodiscard]] Vector4 zyyx() const noexcept { return Vector4<T>(z, y, y, x); }
		[[nodiscard]] Vector4 zyyy() const noexcept { return Vector4<T>(z, y, y, y); }
		[[nodiscard]] Vector4 zyyz() const noexcept { return Vector4<T>(z, y, y, z); }
		[[nodiscard]] Vector4 zyyw() const noexcept { return Vector4<T>(z, y, y, w); }
		[[nodiscard]] Vector4 zyzx() const noexcept { return Vector4<T>(z, y, z, x); }
		[[nodiscard]] Vector4 zyzy() const noexcept { return Vector4<T>(z, y, z, y); }
		[[nodiscard]] Vector4 zyzz() const noexcept { return Vector4<T>(z, y, z, z); }
		[[nodiscard]] Vector4 zyzw() const noexcept { return Vector4<T>(z, y, z, w); }
		[[nodiscard]] Vector4 zywx() const noexcept { return Vector4<T>(z, y, w, x); }
		[[nodiscard]] Vector4 zywy() const noexcept { return Vector4<T>(z, y, w, y); }
		[[nodiscard]] Vector4 zywz() const noexcept { return Vector4<T>(z, y, w, z); }
		[[nodiscard]] Vector4 zyww() const noexcept { return Vector4<T>(z, y, w, w); }
		[[nodiscard]] Vector4 zzxx() const noexcept { return Vector4<T>(z, z, x, x); }
		[[nodiscard]] Vector4 zzxy() const noexcept { return Vector4<T>(z, z, x, y); }
		[[nodiscard]] Vector4 zzxz() const noexcept { return Vector4<T>(z, z, x, z); }
		[[nodiscard]] Vector4 zzxw() const noexcept { return Vector4<T>(z, z, x, w); }
		[[nodiscard]] Vector4 zzyx() const noexcept { return Vector4<T>(z, z, y, x); }
		[[nodiscard]] Vector4 zzyy() const noexcept { return Vector4<T>(z, z, y, y); }
		[[nodiscard]] Vector4 zzyz() const noexcept { return Vector4<T>(z, z, y, z); }
		[[nodiscard]] Vector4 zzyw() const noexcept { return Vector4<T>(z, z, y, w); }
		[[nodiscard]] Vector4 zzzx() const noexcept { return Vector4<T>(z, z, z, x); }
		[[nodiscard]] Vector4 zzzy() const noexcept { return Vector4<T>(z, z, z, y); }
		[[nodiscard]] Vector4 zzzz() const noexcept { return Vector4<T>(z, z, z, z); }
		[[nodiscard]] Vector4 zzzw() const noexcept { return Vector4<T>(z, z, z, w); }
		[[nodiscard]] Vector4 zzwx() const noexcept { return Vector4<T>(z, z, w, x); }
		[[nodiscard]] Vector4 zzwy() const noexcept { return Vector4<T>(z, z, w, y); }
		[[nodiscard]] Vector4 zzwz() const noexcept { return Vector4<T>(z, z, w, z); }
		[[nodiscard]] Vector4 zzww() const noexcept { return Vector4<T>(z, z, w, w); }
		[[nodiscard]] Vector4 zwxx() const noexcept { return Vector4<T>(z, w, x, x); }
		[[nodiscard]] Vector4 zwxy() const noexcept { return Vector4<T>(z, w, x, y); }
		[[nodiscard]] Vector4 zwxz() const noexcept { return Vector4<T>(z, w, x, z); }
		[[nodiscard]] Vector4 zwxw() const noexcept { return Vector4<T>(z, w, x, w); }
		[[nodiscard]] Vector4 zwyx() const noexcept { return Vector4<T>(z, w, y, x); }
		[[nodiscard]] Vector4 zwyy() const noexcept { return Vector4<T>(z, w, y, y); }
		[[nodiscard]] Vector4 zwyz() const noexcept { return Vector4<T>(z, w, y, z); }
		[[nodiscard]] Vector4 zwyw() const noexcept { return Vector4<T>(z, w, y, w); }
		[[nodiscard]] Vector4 zwzx() const noexcept { return Vector4<T>(z, w, z, x); }
		[[nodiscard]] Vector4 zwzy() const noexcept { return Vector4<T>(z, w, z, y); }
		[[nodiscard]] Vector4 zwzz() const noexcept { return Vector4<T>(z, w, z, z); }
		[[nodiscard]] Vector4 zwzw() const noexcept { return Vector4<T>(z, w, z, w); }
		[[nodiscard]] Vector4 zwwx() const noexcept { return Vector4<T>(z, w, w, x); }
		[[nodiscard]] Vector4 zwwy() const noexcept { return Vector4<T>(z, w, w, y); }
		[[nodiscard]] Vector4 zwwz() const noexcept { return Vector4<T>(z, w, w, z); }
		[[nodiscard]] Vector4 zwww() const noexcept { return Vector4<T>(z, w, w, w); }
		[[nodiscard]] Vector4 wxxx() const noexcept { return Vector4<T>(w, x, x, x); }
		[[nodiscard]] Vector4 wxxy() const noexcept { return Vector4<T>(w, x, x, y); }
		[[nodiscard]] Vector4 wxxz() const noexcept { return Vector4<T>(w, x, x, z); }
		[[nodiscard]] Vector4 wxxw() const noexcept { return Vector4<T>(w, x, x, w); }
		[[nodiscard]] Vector4 wxyx() const noexcept { return Vector4<T>(w, x, y, x); }
		[[nodiscard]] Vector4 wxyy() const noexcept { return Vector4<T>(w, x, y, y); }
		[[nodiscard]] Vector4 wxyz() const noexcept { return Vector4<T>(w, x, y, z); }
		[[nodiscard]] Vector4 wxyw() const noexcept { return Vector4<T>(w, x, y, w); }
		[[nodiscard]] Vector4 wxzx() const noexcept { return Vector4<T>(w, x, z, x); }
		[[nodiscard]] Vector4 wxzy() const noexcept { return Vector4<T>(w, x, z, y); }
		[[nodiscard]] Vector4 wxzz() const noexcept { return Vector4<T>(w, x, z, z); }
		[[nodiscard]] Vector4 wxzw() const noexcept { return Vector4<T>(w, x, z, w); }
		[[nodiscard]] Vector4 wxwx() const noexcept { return Vector4<T>(w, x, w, x); }
		[[nodiscard]] Vector4 wxwy() const noexcept { return Vector4<T>(w, x, w, y); }
		[[nodiscard]] Vector4 wxwz() const noexcept { return Vector4<T>(w, x, w, z); }
		[[nodiscard]] Vector4 wxww() const noexcept { return Vector4<T>(w, x, w, w); }
		[[nodiscard]] Vector4 wyxx() const noexcept { return Vector4<T>(w, y, x, x); }
		[[nodiscard]] Vector4 wyxy() const noexcept { return Vector4<T>(w, y, x, y); }
		[[nodiscard]] Vector4 wyxz() const noexcept { return Vector4<T>(w, y, x, z); }
		[[nodiscard]] Vector4 wyxw() const noexcept { return Vector4<T>(w, y, x, w); }
		[[nodiscard]] Vector4 wyyx() const noexcept { return Vector4<T>(w, y, y, x); }
		[[nodiscard]] Vector4 wyyy() const noexcept { return Vector4<T>(w, y, y, y); }
		[[nodiscard]] Vector4 wyyz() const noexcept { return Vector4<T>(w, y, y, z); }
		[[nodiscard]] Vector4 wyyw() const noexcept { return Vector4<T>(w, y, y, w); }
		[[nodiscard]] Vector4 wyzx() const noexcept { return Vector4<T>(w, y, z, x); }
		[[nodiscard]] Vector4 wyzy() const noexcept { return Vector4<T>(w, y, z, y); }
		[[nodiscard]] Vector4 wyzz() const noexcept { return Vector4<T>(w, y, z, z); }
		[[nodiscard]] Vector4 wyzw() const noexcept { return Vector4<T>(w, y, z, w); }
		[[nodiscard]] Vector4 wywx() const noexcept { return Vector4<T>(w, y, w, x); }
		[[nodiscard]] Vector4 wywy() const noexcept { return Vector4<T>(w, y, w, y); }
		[[nodiscard]] Vector4 wywz() const noexcept { return Vector4<T>(w, y, w, z); }
		[[nodiscard]] Vector4 wyww() const noexcept { return Vector4<T>(w, y, w, w); }
		[[nodiscard]] Vector4 wzxx() const noexcept { return Vector4<T>(w, z, x, x); }
		[[nodiscard]] Vector4 wzxy() const noexcept { return Vector4<T>(w, z, x, y); }
		[[nodiscard]] Vector4 wzxz() const noexcept { return Vector4<T>(w, z, x, z); }
		[[nodiscard]] Vector4 wzxw() const noexcept { return Vector4<T>(w, z, x, w); }
		[[nodiscard]] Vector4 wzyx() const noexcept { return Vector4<T>(w, z, y, x); }
		[[nodiscard]] Vector4 wzyy() const noexcept { return Vector4<T>(w, z, y, y); }
		[[nodiscard]] Vector4 wzyz() const noexcept { return Vector4<T>(w, z, y, z); }
		[[nodiscard]] Vector4 wzyw() const noexcept { return Vector4<T>(w, z, y, w); }
		[[nodiscard]] Vector4 wzzx() const noexcept { return Vector4<T>(w, z, z, x); }
		[[nodiscard]] Vector4 wzzy() const noexcept { return Vector4<T>(w, z, z, y); }
		[[nodiscard]] Vector4 wzzz() const noexcept { return Vector4<T>(w, z, z, z); }
		[[nodiscard]] Vector4 wzzw() const noexcept { return Vector4<T>(w, z, z, w); }
		[[nodiscard]] Vector4 wzwx() const noexcept { return Vector4<T>(w, z, w, x); }
		[[nodiscard]] Vector4 wzwy() const noexcept { return Vector4<T>(w, z, w, y); }
		[[nodiscard]] Vector4 wzwz() const noexcept { return Vector4<T>(w, z, w, z); }
		[[nodiscard]] Vector4 wzww() const noexcept { return Vector4<T>(w, z, w, w); }
		[[nodiscard]] Vector4 wwxx() const noexcept { return Vector4<T>(w, w, x, x); }
		[[nodiscard]] Vector4 wwxy() const noexcept { return Vector4<T>(w, w, x, y); }
		[[nodiscard]] Vector4 wwxz() const noexcept { return Vector4<T>(w, w, x, z); }
		[[nodiscard]] Vector4 wwxw() const noexcept { return Vector4<T>(w, w, x, w); }
		[[nodiscard]] Vector4 wwyx() const noexcept { return Vector4<T>(w, w, y, x); }
		[[nodiscard]] Vector4 wwyy() const noexcept { return Vector4<T>(w, w, y, y); }
		[[nodiscard]] Vector4 wwyz() const noexcept { return Vector4<T>(w, w, y, z); }
		[[nodiscard]] Vector4 wwyw() const noexcept { return Vector4<T>(w, w, y, w); }
		[[nodiscard]] Vector4 wwzx() const noexcept { return Vector4<T>(w, w, z, x); }
		[[nodiscard]] Vector4 wwzy() const noexcept { return Vector4<T>(w, w, z, y); }
		[[nodiscard]] Vector4 wwzz() const noexcept { return Vector4<T>(w, w, z, z); }
		[[nodiscard]] Vector4 wwzw() const noexcept { return Vector4<T>(w, w, z, w); }
		[[nodiscard]] Vector4 wwwx() const noexcept { return Vector4<T>(w, w, w, x); }
		[[nodiscard]] Vector4 wwwy() const noexcept { return Vector4<T>(w, w, w, y); }
		[[nodiscard]] Vector4 wwwz() const noexcept { return Vector4<T>(w, w, w, z); }
		[[nodiscard]] Vector4 wwww() const noexcept { return Vector4<T>(w, w, w, w); }

#pragma endregion

		[[nodiscard]] T min() const noexcept;
		[[nodiscard]] T max() const noexcept;
		[[nodiscard]] T magnitude_sqr() const noexcept;
		[[nodiscard]] F magnitude() const noexcept;
		[[nodiscard]] Vector4<F> normalized() const noexcept;
		[[nodiscard]] Vector4<F> normalized_unsafe() const noexcept;
		[[nodiscard]] Vector4<F> reciprocal() const noexcept;
		[[nodiscard]] Vector4 abs() const noexcept;

		bool operator==(const Vector4& other) const;
		bool operator!=(const Vector4& other) const;

		Vector4 operator+() const;
		Vector4 operator-() const;

		Vector4& operator+=(const Vector4& other);
		Vector4& operator-=(const Vector4& other);
		Vector4& operator*=(const Vector4& other);
		Vector4& operator/=(const Vector4& other);
		Vector4 operator+(const Vector4& other) const;
		Vector4 operator-(const Vector4& other) const;
		Vector4 operator*(const Vector4& other) const;
		Vector4 operator/(const Vector4& other) const;

		Vector4& operator*=(const T& scalar);
		Vector4& operator/=(const T& scalar);
		Vector4 operator*(const T& scalar) const;
		Vector4 operator/(const T& scalar) const;

		static constexpr Vector4 zero() { return Vector4(0, 0, 0, 0); }
		static constexpr Vector4 one() { return Vector4(1, 1, 1, 1); }
	};

	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4i = Vector4<int32_t>;
	using Vector4u = Vector4<uint32_t>;
	using Vector4u8 = Vector4<uint8_t>;

	template <number T>
	Vector4<T>::Vector4()
		: x(0)
		, y(0)
		, z(0)
		, w(0)
	{ }

	template <number T>
	Vector4<T>::Vector4(T x, T y, T z, T w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{ }

	template <number T>
	template <number U>
	requires std::convertible_to<U, T>
	Vector4<T>::Vector4(const Vector4<U>& other)
		: x(static_cast<T>(other.x))
		, y(static_cast<T>(other.y))
		, z(static_cast<T>(other.z))
		, w(static_cast<T>(other.w))
	{ }

	template <number T>
	T Vector4<T>::min() const noexcept
	{
		return std::min(x, y, z, w);
	}

	template <number T>
	T Vector4<T>::max() const noexcept
	{
		return std::max(x, y, z, w);
	}

	template <number T>
	T Vector4<T>::magnitude_sqr() const noexcept
	{
		return x * x + y * y + z * z + w * w;
	}

	template <number T>
	make_floating_t<T> Vector4<T>::magnitude() const noexcept
	{
		return std::sqrt(static_cast<F>(magnitude_sqr()));
	}

	template <number T>
	Vector4<make_floating_t<T>> Vector4<T>::normalized() const noexcept
	{
		const F mag = magnitude();
		if (mag != 0)
		{
			return static_cast<Vector4<F>>(*this) / mag;
		}

		return *this;
	}

	template <number T>
	Vector4<make_floating_t<T>> Vector4<T>::normalized_unsafe() const noexcept
	{
		return static_cast<Vector4<F>>(*this) / magnitude();
	}

	template <number T>
	Vector4<make_floating_t<T>> Vector4<T>::reciprocal() const noexcept
	{
		constexpr F one_f = 1;
		return Vector4<F>(one_f / x, one_f / y, one_f / z, one_f / w);
	}

	template <number T>
	Vector4<T> Vector4<T>::abs() const noexcept
	{
		return Vector4(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
	}

	template <number T>
	bool Vector4<T>::operator==(const Vector4& other) const
	{
		return x == other.x
			&& y == other.y
			&& z == other.z
			&& w == other.w;
	}

	template <number T>
	bool Vector4<T>::operator!=(const Vector4& other) const
	{
		return !(*this == other);
	}

	template <number T>
	Vector4<T> Vector4<T>::operator+() const
	{
		return *this;
	}

	template <number T>
	Vector4<T> Vector4<T>::operator-() const
	{
		return *this * -1;
	}

	template <number T>
	Vector4<T>& Vector4<T>::operator+=(const Vector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	template <number T>
	Vector4<T>& Vector4<T>::operator-=(const Vector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	template <number T>
	Vector4<T>& Vector4<T>::operator*=(const Vector4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}

	template <number T>
	Vector4<T>& Vector4<T>::operator/=(const Vector4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}

	template <number T>
	Vector4<T> Vector4<T>::operator+(const Vector4& other) const
	{
		Vector4 result = *this;
		return result += other;
	}

	template <number T>
	Vector4<T> Vector4<T>::operator-(const Vector4& other) const
	{
		Vector4 result = *this;
		return result -= other;
	}

	template <number T>
	Vector4<T> Vector4<T>::operator*(const Vector4& other) const
	{
		Vector4 result = *this;
		return result *= other;
	}

	template <number T>
	Vector4<T> Vector4<T>::operator/(const Vector4& other) const
	{
		Vector4 result = *this;
		return result /= other;
	}

	template <number T>
	Vector4<T>& Vector4<T>::operator*=(const T& scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	template <number T>
	Vector4<T>& Vector4<T>::operator/=(const T& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;

		return *this;
	}

	template <number T>
	Vector4<T> Vector4<T>::operator*(const T& scalar) const
	{
		Vector4 result = *this;
		return result *= scalar;
	}

	template <number T>
	Vector4<T> Vector4<T>::operator/(const T& scalar) const
	{
		Vector4 result = *this;
		return result /= scalar;
	}
}

template<math::number T>
struct std::hash<math::Vector4<T>>
{
	size_t operator()(const math::Vector3<T>& x) const
	{
		return std::hash<T>{}(x.x) ^ std::hash<T>{}(x.y) ^ std::hash<T>{}(x.z) ^ std::hash<T>{}(x.w);
	}
};