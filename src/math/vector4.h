#pragma once

#include "vector3.h"

namespace math
{
	template <number T>
	struct Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		Vector4();
		Vector4(T x, T y, T z, T w);

#pragma region Swizzling

		Vector2<T> xx() const noexcept { return Vector2<T>(x, x); }
		Vector2<T> xy() const noexcept { return Vector2<T>(x, y); }
		Vector2<T> yx() const noexcept { return Vector2<T>(y, x); }
		Vector2<T> yy() const noexcept { return Vector2<T>(y, y); }

		Vector3<T> xxx() const noexcept { return Vector3<T>(x, x, x); }
		Vector3<T> xxy() const noexcept { return Vector3<T>(x, x, y); }
		Vector3<T> xxz() const noexcept { return Vector3<T>(x, x, z); }
		Vector3<T> xyx() const noexcept { return Vector3<T>(x, y, x); }
		Vector3<T> xyy() const noexcept { return Vector3<T>(x, y, y); }
		Vector3<T> xyz() const noexcept { return Vector3<T>(x, y, z); }
		Vector3<T> xzx() const noexcept { return Vector3<T>(x, z, x); }
		Vector3<T> xzy() const noexcept { return Vector3<T>(x, z, y); }
		Vector3<T> xzz() const noexcept { return Vector3<T>(x, z, z); }
		Vector3<T> yxx() const noexcept { return Vector3<T>(y, x, x); }
		Vector3<T> yxy() const noexcept { return Vector3<T>(y, x, y); }
		Vector3<T> yxz() const noexcept { return Vector3<T>(y, x, z); }
		Vector3<T> yyx() const noexcept { return Vector3<T>(y, y, x); }
		Vector3<T> yyy() const noexcept { return Vector3<T>(y, y, y); }
		Vector3<T> yyz() const noexcept { return Vector3<T>(y, y, z); }
		Vector3<T> yzx() const noexcept { return Vector3<T>(y, z, x); }
		Vector3<T> yzy() const noexcept { return Vector3<T>(y, z, y); }
		Vector3<T> yzz() const noexcept { return Vector3<T>(y, z, z); }
		Vector3<T> zxx() const noexcept { return Vector3<T>(z, x, x); }
		Vector3<T> zxy() const noexcept { return Vector3<T>(z, x, y); }
		Vector3<T> zxz() const noexcept { return Vector3<T>(z, x, z); }
		Vector3<T> zyx() const noexcept { return Vector3<T>(z, y, x); }
		Vector3<T> zyy() const noexcept { return Vector3<T>(z, y, y); }
		Vector3<T> zyz() const noexcept { return Vector3<T>(z, y, z); }
		Vector3<T> zzx() const noexcept { return Vector3<T>(z, z, x); }
		Vector3<T> zzy() const noexcept { return Vector3<T>(z, z, y); }
		Vector3<T> zzz() const noexcept { return Vector3<T>(z, z, z); }

		Vector4 xxxx() const noexcept { return Vector4<T>(x, x, x, x); }
		Vector4 xxxy() const noexcept { return Vector4<T>(x, x, x, y); }
		Vector4 xxxz() const noexcept { return Vector4<T>(x, x, x, z); }
		Vector4 xxxw() const noexcept { return Vector4<T>(x, x, x, w); }
		Vector4 xxyx() const noexcept { return Vector4<T>(x, x, y, x); }
		Vector4 xxyy() const noexcept { return Vector4<T>(x, x, y, y); }
		Vector4 xxyz() const noexcept { return Vector4<T>(x, x, y, z); }
		Vector4 xxyw() const noexcept { return Vector4<T>(x, x, y, w); }
		Vector4 xxzx() const noexcept { return Vector4<T>(x, x, z, x); }
		Vector4 xxzy() const noexcept { return Vector4<T>(x, x, z, y); }
		Vector4 xxzz() const noexcept { return Vector4<T>(x, x, z, z); }
		Vector4 xxzw() const noexcept { return Vector4<T>(x, x, z, w); }
		Vector4 xxwx() const noexcept { return Vector4<T>(x, x, w, x); }
		Vector4 xxwy() const noexcept { return Vector4<T>(x, x, w, y); }
		Vector4 xxwz() const noexcept { return Vector4<T>(x, x, w, z); }
		Vector4 xxww() const noexcept { return Vector4<T>(x, x, w, w); }
		Vector4 xyxx() const noexcept { return Vector4<T>(x, y, x, x); }
		Vector4 xyxy() const noexcept { return Vector4<T>(x, y, x, y); }
		Vector4 xyxz() const noexcept { return Vector4<T>(x, y, x, z); }
		Vector4 xyxw() const noexcept { return Vector4<T>(x, y, x, w); }
		Vector4 xyyx() const noexcept { return Vector4<T>(x, y, y, x); }
		Vector4 xyyy() const noexcept { return Vector4<T>(x, y, y, y); }
		Vector4 xyyz() const noexcept { return Vector4<T>(x, y, y, z); }
		Vector4 xyyw() const noexcept { return Vector4<T>(x, y, y, w); }
		Vector4 xyzx() const noexcept { return Vector4<T>(x, y, z, x); }
		Vector4 xyzy() const noexcept { return Vector4<T>(x, y, z, y); }
		Vector4 xyzz() const noexcept { return Vector4<T>(x, y, z, z); }
		Vector4 xyzw() const noexcept { return Vector4<T>(x, y, z, w); }
		Vector4 xywx() const noexcept { return Vector4<T>(x, y, w, x); }
		Vector4 xywy() const noexcept { return Vector4<T>(x, y, w, y); }
		Vector4 xywz() const noexcept { return Vector4<T>(x, y, w, z); }
		Vector4 xyww() const noexcept { return Vector4<T>(x, y, w, w); }
		Vector4 xzxx() const noexcept { return Vector4<T>(x, z, x, x); }
		Vector4 xzxy() const noexcept { return Vector4<T>(x, z, x, y); }
		Vector4 xzxz() const noexcept { return Vector4<T>(x, z, x, z); }
		Vector4 xzxw() const noexcept { return Vector4<T>(x, z, x, w); }
		Vector4 xzyx() const noexcept { return Vector4<T>(x, z, y, x); }
		Vector4 xzyy() const noexcept { return Vector4<T>(x, z, y, y); }
		Vector4 xzyz() const noexcept { return Vector4<T>(x, z, y, z); }
		Vector4 xzyw() const noexcept { return Vector4<T>(x, z, y, w); }
		Vector4 xzzx() const noexcept { return Vector4<T>(x, z, z, x); }
		Vector4 xzzy() const noexcept { return Vector4<T>(x, z, z, y); }
		Vector4 xzzz() const noexcept { return Vector4<T>(x, z, z, z); }
		Vector4 xzzw() const noexcept { return Vector4<T>(x, z, z, w); }
		Vector4 xzwx() const noexcept { return Vector4<T>(x, z, w, x); }
		Vector4 xzwy() const noexcept { return Vector4<T>(x, z, w, y); }
		Vector4 xzwz() const noexcept { return Vector4<T>(x, z, w, z); }
		Vector4 xzww() const noexcept { return Vector4<T>(x, z, w, w); }
		Vector4 xwxx() const noexcept { return Vector4<T>(x, w, x, x); }
		Vector4 xwxy() const noexcept { return Vector4<T>(x, w, x, y); }
		Vector4 xwxz() const noexcept { return Vector4<T>(x, w, x, z); }
		Vector4 xwxw() const noexcept { return Vector4<T>(x, w, x, w); }
		Vector4 xwyx() const noexcept { return Vector4<T>(x, w, y, x); }
		Vector4 xwyy() const noexcept { return Vector4<T>(x, w, y, y); }
		Vector4 xwyz() const noexcept { return Vector4<T>(x, w, y, z); }
		Vector4 xwyw() const noexcept { return Vector4<T>(x, w, y, w); }
		Vector4 xwzx() const noexcept { return Vector4<T>(x, w, z, x); }
		Vector4 xwzy() const noexcept { return Vector4<T>(x, w, z, y); }
		Vector4 xwzz() const noexcept { return Vector4<T>(x, w, z, z); }
		Vector4 xwzw() const noexcept { return Vector4<T>(x, w, z, w); }
		Vector4 xwwx() const noexcept { return Vector4<T>(x, w, w, x); }
		Vector4 xwwy() const noexcept { return Vector4<T>(x, w, w, y); }
		Vector4 xwwz() const noexcept { return Vector4<T>(x, w, w, z); }
		Vector4 xwww() const noexcept { return Vector4<T>(x, w, w, w); }
		Vector4 yxxx() const noexcept { return Vector4<T>(y, x, x, x); }
		Vector4 yxxy() const noexcept { return Vector4<T>(y, x, x, y); }
		Vector4 yxxz() const noexcept { return Vector4<T>(y, x, x, z); }
		Vector4 yxxw() const noexcept { return Vector4<T>(y, x, x, w); }
		Vector4 yxyx() const noexcept { return Vector4<T>(y, x, y, x); }
		Vector4 yxyy() const noexcept { return Vector4<T>(y, x, y, y); }
		Vector4 yxyz() const noexcept { return Vector4<T>(y, x, y, z); }
		Vector4 yxyw() const noexcept { return Vector4<T>(y, x, y, w); }
		Vector4 yxzx() const noexcept { return Vector4<T>(y, x, z, x); }
		Vector4 yxzy() const noexcept { return Vector4<T>(y, x, z, y); }
		Vector4 yxzz() const noexcept { return Vector4<T>(y, x, z, z); }
		Vector4 yxzw() const noexcept { return Vector4<T>(y, x, z, w); }
		Vector4 yxwx() const noexcept { return Vector4<T>(y, x, w, x); }
		Vector4 yxwy() const noexcept { return Vector4<T>(y, x, w, y); }
		Vector4 yxwz() const noexcept { return Vector4<T>(y, x, w, z); }
		Vector4 yxww() const noexcept { return Vector4<T>(y, x, w, w); }
		Vector4 yyxx() const noexcept { return Vector4<T>(y, y, x, x); }
		Vector4 yyxy() const noexcept { return Vector4<T>(y, y, x, y); }
		Vector4 yyxz() const noexcept { return Vector4<T>(y, y, x, z); }
		Vector4 yyxw() const noexcept { return Vector4<T>(y, y, x, w); }
		Vector4 yyyx() const noexcept { return Vector4<T>(y, y, y, x); }
		Vector4 yyyy() const noexcept { return Vector4<T>(y, y, y, y); }
		Vector4 yyyz() const noexcept { return Vector4<T>(y, y, y, z); }
		Vector4 yyyw() const noexcept { return Vector4<T>(y, y, y, w); }
		Vector4 yyzx() const noexcept { return Vector4<T>(y, y, z, x); }
		Vector4 yyzy() const noexcept { return Vector4<T>(y, y, z, y); }
		Vector4 yyzz() const noexcept { return Vector4<T>(y, y, z, z); }
		Vector4 yyzw() const noexcept { return Vector4<T>(y, y, z, w); }
		Vector4 yywx() const noexcept { return Vector4<T>(y, y, w, x); }
		Vector4 yywy() const noexcept { return Vector4<T>(y, y, w, y); }
		Vector4 yywz() const noexcept { return Vector4<T>(y, y, w, z); }
		Vector4 yyww() const noexcept { return Vector4<T>(y, y, w, w); }
		Vector4 yzxx() const noexcept { return Vector4<T>(y, z, x, x); }
		Vector4 yzxy() const noexcept { return Vector4<T>(y, z, x, y); }
		Vector4 yzxz() const noexcept { return Vector4<T>(y, z, x, z); }
		Vector4 yzxw() const noexcept { return Vector4<T>(y, z, x, w); }
		Vector4 yzyx() const noexcept { return Vector4<T>(y, z, y, x); }
		Vector4 yzyy() const noexcept { return Vector4<T>(y, z, y, y); }
		Vector4 yzyz() const noexcept { return Vector4<T>(y, z, y, z); }
		Vector4 yzyw() const noexcept { return Vector4<T>(y, z, y, w); }
		Vector4 yzzx() const noexcept { return Vector4<T>(y, z, z, x); }
		Vector4 yzzy() const noexcept { return Vector4<T>(y, z, z, y); }
		Vector4 yzzz() const noexcept { return Vector4<T>(y, z, z, z); }
		Vector4 yzzw() const noexcept { return Vector4<T>(y, z, z, w); }
		Vector4 yzwx() const noexcept { return Vector4<T>(y, z, w, x); }
		Vector4 yzwy() const noexcept { return Vector4<T>(y, z, w, y); }
		Vector4 yzwz() const noexcept { return Vector4<T>(y, z, w, z); }
		Vector4 yzww() const noexcept { return Vector4<T>(y, z, w, w); }
		Vector4 ywxx() const noexcept { return Vector4<T>(y, w, x, x); }
		Vector4 ywxy() const noexcept { return Vector4<T>(y, w, x, y); }
		Vector4 ywxz() const noexcept { return Vector4<T>(y, w, x, z); }
		Vector4 ywxw() const noexcept { return Vector4<T>(y, w, x, w); }
		Vector4 ywyx() const noexcept { return Vector4<T>(y, w, y, x); }
		Vector4 ywyy() const noexcept { return Vector4<T>(y, w, y, y); }
		Vector4 ywyz() const noexcept { return Vector4<T>(y, w, y, z); }
		Vector4 ywyw() const noexcept { return Vector4<T>(y, w, y, w); }
		Vector4 ywzx() const noexcept { return Vector4<T>(y, w, z, x); }
		Vector4 ywzy() const noexcept { return Vector4<T>(y, w, z, y); }
		Vector4 ywzz() const noexcept { return Vector4<T>(y, w, z, z); }
		Vector4 ywzw() const noexcept { return Vector4<T>(y, w, z, w); }
		Vector4 ywwx() const noexcept { return Vector4<T>(y, w, w, x); }
		Vector4 ywwy() const noexcept { return Vector4<T>(y, w, w, y); }
		Vector4 ywwz() const noexcept { return Vector4<T>(y, w, w, z); }
		Vector4 ywww() const noexcept { return Vector4<T>(y, w, w, w); }
		Vector4 zxxx() const noexcept { return Vector4<T>(z, x, x, x); }
		Vector4 zxxy() const noexcept { return Vector4<T>(z, x, x, y); }
		Vector4 zxxz() const noexcept { return Vector4<T>(z, x, x, z); }
		Vector4 zxxw() const noexcept { return Vector4<T>(z, x, x, w); }
		Vector4 zxyx() const noexcept { return Vector4<T>(z, x, y, x); }
		Vector4 zxyy() const noexcept { return Vector4<T>(z, x, y, y); }
		Vector4 zxyz() const noexcept { return Vector4<T>(z, x, y, z); }
		Vector4 zxyw() const noexcept { return Vector4<T>(z, x, y, w); }
		Vector4 zxzx() const noexcept { return Vector4<T>(z, x, z, x); }
		Vector4 zxzy() const noexcept { return Vector4<T>(z, x, z, y); }
		Vector4 zxzz() const noexcept { return Vector4<T>(z, x, z, z); }
		Vector4 zxzw() const noexcept { return Vector4<T>(z, x, z, w); }
		Vector4 zxwx() const noexcept { return Vector4<T>(z, x, w, x); }
		Vector4 zxwy() const noexcept { return Vector4<T>(z, x, w, y); }
		Vector4 zxwz() const noexcept { return Vector4<T>(z, x, w, z); }
		Vector4 zxww() const noexcept { return Vector4<T>(z, x, w, w); }
		Vector4 zyxx() const noexcept { return Vector4<T>(z, y, x, x); }
		Vector4 zyxy() const noexcept { return Vector4<T>(z, y, x, y); }
		Vector4 zyxz() const noexcept { return Vector4<T>(z, y, x, z); }
		Vector4 zyxw() const noexcept { return Vector4<T>(z, y, x, w); }
		Vector4 zyyx() const noexcept { return Vector4<T>(z, y, y, x); }
		Vector4 zyyy() const noexcept { return Vector4<T>(z, y, y, y); }
		Vector4 zyyz() const noexcept { return Vector4<T>(z, y, y, z); }
		Vector4 zyyw() const noexcept { return Vector4<T>(z, y, y, w); }
		Vector4 zyzx() const noexcept { return Vector4<T>(z, y, z, x); }
		Vector4 zyzy() const noexcept { return Vector4<T>(z, y, z, y); }
		Vector4 zyzz() const noexcept { return Vector4<T>(z, y, z, z); }
		Vector4 zyzw() const noexcept { return Vector4<T>(z, y, z, w); }
		Vector4 zywx() const noexcept { return Vector4<T>(z, y, w, x); }
		Vector4 zywy() const noexcept { return Vector4<T>(z, y, w, y); }
		Vector4 zywz() const noexcept { return Vector4<T>(z, y, w, z); }
		Vector4 zyww() const noexcept { return Vector4<T>(z, y, w, w); }
		Vector4 zzxx() const noexcept { return Vector4<T>(z, z, x, x); }
		Vector4 zzxy() const noexcept { return Vector4<T>(z, z, x, y); }
		Vector4 zzxz() const noexcept { return Vector4<T>(z, z, x, z); }
		Vector4 zzxw() const noexcept { return Vector4<T>(z, z, x, w); }
		Vector4 zzyx() const noexcept { return Vector4<T>(z, z, y, x); }
		Vector4 zzyy() const noexcept { return Vector4<T>(z, z, y, y); }
		Vector4 zzyz() const noexcept { return Vector4<T>(z, z, y, z); }
		Vector4 zzyw() const noexcept { return Vector4<T>(z, z, y, w); }
		Vector4 zzzx() const noexcept { return Vector4<T>(z, z, z, x); }
		Vector4 zzzy() const noexcept { return Vector4<T>(z, z, z, y); }
		Vector4 zzzz() const noexcept { return Vector4<T>(z, z, z, z); }
		Vector4 zzzw() const noexcept { return Vector4<T>(z, z, z, w); }
		Vector4 zzwx() const noexcept { return Vector4<T>(z, z, w, x); }
		Vector4 zzwy() const noexcept { return Vector4<T>(z, z, w, y); }
		Vector4 zzwz() const noexcept { return Vector4<T>(z, z, w, z); }
		Vector4 zzww() const noexcept { return Vector4<T>(z, z, w, w); }
		Vector4 zwxx() const noexcept { return Vector4<T>(z, w, x, x); }
		Vector4 zwxy() const noexcept { return Vector4<T>(z, w, x, y); }
		Vector4 zwxz() const noexcept { return Vector4<T>(z, w, x, z); }
		Vector4 zwxw() const noexcept { return Vector4<T>(z, w, x, w); }
		Vector4 zwyx() const noexcept { return Vector4<T>(z, w, y, x); }
		Vector4 zwyy() const noexcept { return Vector4<T>(z, w, y, y); }
		Vector4 zwyz() const noexcept { return Vector4<T>(z, w, y, z); }
		Vector4 zwyw() const noexcept { return Vector4<T>(z, w, y, w); }
		Vector4 zwzx() const noexcept { return Vector4<T>(z, w, z, x); }
		Vector4 zwzy() const noexcept { return Vector4<T>(z, w, z, y); }
		Vector4 zwzz() const noexcept { return Vector4<T>(z, w, z, z); }
		Vector4 zwzw() const noexcept { return Vector4<T>(z, w, z, w); }
		Vector4 zwwx() const noexcept { return Vector4<T>(z, w, w, x); }
		Vector4 zwwy() const noexcept { return Vector4<T>(z, w, w, y); }
		Vector4 zwwz() const noexcept { return Vector4<T>(z, w, w, z); }
		Vector4 zwww() const noexcept { return Vector4<T>(z, w, w, w); }
		Vector4 wxxx() const noexcept { return Vector4<T>(w, x, x, x); }
		Vector4 wxxy() const noexcept { return Vector4<T>(w, x, x, y); }
		Vector4 wxxz() const noexcept { return Vector4<T>(w, x, x, z); }
		Vector4 wxxw() const noexcept { return Vector4<T>(w, x, x, w); }
		Vector4 wxyx() const noexcept { return Vector4<T>(w, x, y, x); }
		Vector4 wxyy() const noexcept { return Vector4<T>(w, x, y, y); }
		Vector4 wxyz() const noexcept { return Vector4<T>(w, x, y, z); }
		Vector4 wxyw() const noexcept { return Vector4<T>(w, x, y, w); }
		Vector4 wxzx() const noexcept { return Vector4<T>(w, x, z, x); }
		Vector4 wxzy() const noexcept { return Vector4<T>(w, x, z, y); }
		Vector4 wxzz() const noexcept { return Vector4<T>(w, x, z, z); }
		Vector4 wxzw() const noexcept { return Vector4<T>(w, x, z, w); }
		Vector4 wxwx() const noexcept { return Vector4<T>(w, x, w, x); }
		Vector4 wxwy() const noexcept { return Vector4<T>(w, x, w, y); }
		Vector4 wxwz() const noexcept { return Vector4<T>(w, x, w, z); }
		Vector4 wxww() const noexcept { return Vector4<T>(w, x, w, w); }
		Vector4 wyxx() const noexcept { return Vector4<T>(w, y, x, x); }
		Vector4 wyxy() const noexcept { return Vector4<T>(w, y, x, y); }
		Vector4 wyxz() const noexcept { return Vector4<T>(w, y, x, z); }
		Vector4 wyxw() const noexcept { return Vector4<T>(w, y, x, w); }
		Vector4 wyyx() const noexcept { return Vector4<T>(w, y, y, x); }
		Vector4 wyyy() const noexcept { return Vector4<T>(w, y, y, y); }
		Vector4 wyyz() const noexcept { return Vector4<T>(w, y, y, z); }
		Vector4 wyyw() const noexcept { return Vector4<T>(w, y, y, w); }
		Vector4 wyzx() const noexcept { return Vector4<T>(w, y, z, x); }
		Vector4 wyzy() const noexcept { return Vector4<T>(w, y, z, y); }
		Vector4 wyzz() const noexcept { return Vector4<T>(w, y, z, z); }
		Vector4 wyzw() const noexcept { return Vector4<T>(w, y, z, w); }
		Vector4 wywx() const noexcept { return Vector4<T>(w, y, w, x); }
		Vector4 wywy() const noexcept { return Vector4<T>(w, y, w, y); }
		Vector4 wywz() const noexcept { return Vector4<T>(w, y, w, z); }
		Vector4 wyww() const noexcept { return Vector4<T>(w, y, w, w); }
		Vector4 wzxx() const noexcept { return Vector4<T>(w, z, x, x); }
		Vector4 wzxy() const noexcept { return Vector4<T>(w, z, x, y); }
		Vector4 wzxz() const noexcept { return Vector4<T>(w, z, x, z); }
		Vector4 wzxw() const noexcept { return Vector4<T>(w, z, x, w); }
		Vector4 wzyx() const noexcept { return Vector4<T>(w, z, y, x); }
		Vector4 wzyy() const noexcept { return Vector4<T>(w, z, y, y); }
		Vector4 wzyz() const noexcept { return Vector4<T>(w, z, y, z); }
		Vector4 wzyw() const noexcept { return Vector4<T>(w, z, y, w); }
		Vector4 wzzx() const noexcept { return Vector4<T>(w, z, z, x); }
		Vector4 wzzy() const noexcept { return Vector4<T>(w, z, z, y); }
		Vector4 wzzz() const noexcept { return Vector4<T>(w, z, z, z); }
		Vector4 wzzw() const noexcept { return Vector4<T>(w, z, z, w); }
		Vector4 wzwx() const noexcept { return Vector4<T>(w, z, w, x); }
		Vector4 wzwy() const noexcept { return Vector4<T>(w, z, w, y); }
		Vector4 wzwz() const noexcept { return Vector4<T>(w, z, w, z); }
		Vector4 wzww() const noexcept { return Vector4<T>(w, z, w, w); }
		Vector4 wwxx() const noexcept { return Vector4<T>(w, w, x, x); }
		Vector4 wwxy() const noexcept { return Vector4<T>(w, w, x, y); }
		Vector4 wwxz() const noexcept { return Vector4<T>(w, w, x, z); }
		Vector4 wwxw() const noexcept { return Vector4<T>(w, w, x, w); }
		Vector4 wwyx() const noexcept { return Vector4<T>(w, w, y, x); }
		Vector4 wwyy() const noexcept { return Vector4<T>(w, w, y, y); }
		Vector4 wwyz() const noexcept { return Vector4<T>(w, w, y, z); }
		Vector4 wwyw() const noexcept { return Vector4<T>(w, w, y, w); }
		Vector4 wwzx() const noexcept { return Vector4<T>(w, w, z, x); }
		Vector4 wwzy() const noexcept { return Vector4<T>(w, w, z, y); }
		Vector4 wwzz() const noexcept { return Vector4<T>(w, w, z, z); }
		Vector4 wwzw() const noexcept { return Vector4<T>(w, w, z, w); }
		Vector4 wwwx() const noexcept { return Vector4<T>(w, w, w, x); }
		Vector4 wwwy() const noexcept { return Vector4<T>(w, w, w, y); }
		Vector4 wwwz() const noexcept { return Vector4<T>(w, w, w, z); }
		Vector4 wwww() const noexcept { return Vector4<T>(w, w, w, w); }

#pragma endregion

		Vector4& operator+=(const Vector4& other);
		Vector4& operator-=(const Vector4& other);
		Vector4 operator+(const Vector4& other);
		Vector4 operator-(const Vector4& other);
	};

	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4i = Vector4<int32_t>;
	using Vector4u = Vector4<uint32_t>;

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
	Vector4<T> Vector4<T>::operator+(const Vector4& other)
	{
		Vector4 result = *this;
		return result += other;
	}

	template <number T>
	Vector4<T> Vector4<T>::operator-(const Vector4& other)
	{
		Vector4 result = *this;
		return result -= other;
	}
}