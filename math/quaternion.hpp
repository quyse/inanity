#ifndef ___INANITY_MATH_QUATERNION_HPP___
#define ___INANITY_MATH_QUATERNION_HPP___

#include "basic.hpp"

BEGIN_INANITY_MATH

/// Кватернион.
/**
Вращение вокруг оси:
(x,y,z) = ось вращения * sin(alpha/2)
w = cos(alpha/2)
*/
struct quaternion : float4
{
	/// Конструктор по умолчанию - кватернион без поворота.
	inline quaternion() : float4(0, 0, 0, 1) {}

	/// Конструктор из параметров.
	inline quaternion(float x, float y, float z, float w)
	: float4(x, y, z, w) {}

	/// Конструктор из оси и угла.
	inline quaternion(const float3& axis, float angle)
	{
		angle /= 2;
		float angleSin = sin(angle);
		float angleCos = cos(angle);
		x = axis.x * angleSin;
		y = axis.y * angleSin;
		z = axis.z * angleSin;
		w = angleCos;
	}

	/// Явный конструктор из float4.
	inline explicit quaternion(const float4& v)
	: float4(v) {}

	/// Оператор присваивания.
	inline quaternion& operator=(const float4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	/// Инверсия.
	inline quaternion conjugate() const
	{
		return quaternion(-x, -y, -z, w);
	}

	/// Квадрат длины кватерниона.
	inline float length2() const
	{
		return x * x + y * y + z * z + w * w;
	}

	/// Длина кватерниона.
	inline float length() const
	{
		return sqrt(length2());
	}

	/// Нормализация кватерниона.
	inline quaternion normalize() const
	{
		float invLength = 1.0f / length();
		return quaternion(x * invLength, y * invLength, z * invLength, w * invLength);
	}

	/// Матрица поворота.
	operator float4x4() const
	{
		float4x4 r;
		r.t[0][0] = 1 - (y * y + z * z) * 2;
		r.t[0][1] = (x * y - w * z) * 2;
		r.t[0][2] = (x * z + w * y) * 2;
		r.t[0][3] = 0;
		r.t[1][0] = (x * y + w * z) * 2;
		r.t[1][1] = 1 - (x * x + z * z) * 2;
		r.t[1][2] = (y * z - w * x) * 2;
		r.t[1][3] = 0;
		r.t[2][0] = (x * z - w * y) * 2;
		r.t[2][1] = (y * z + w * x) * 2;
		r.t[2][2] = 1 - (x * x + y * y) * 2;
		r.t[2][3] = 0;
		r.t[3][0] = 0;
		r.t[3][1] = 0;
		r.t[3][2] = 0;
		r.t[3][3] = 1;
		return r;
	}
};

/// Умножение кватернионов.
/* ij = k, jk = i, ki = j
*/
inline quaternion operator*(const quaternion& a, const quaternion& b)
{
#if 1
	return quaternion(
		 a.x * b.w + a.y * b.z - a.z * b.y + a.w * b.x,
		-a.x * b.z + a.y * b.w + a.z * b.x + a.w * b.y,
		 a.x * b.y - a.y * b.x + a.z * b.w + a.w * b.z,
		-a.x * b.x - a.y * b.y - a.z * b.z + a.w * b.w);
#else
	// более эффективное вычисление
	float A = (a.w + a.x) * (b.w + b.x);
	float B = (a.z - a.y) * (b.y - b.z);
	float C = (a.x - a.w) * (b.y + b.z);
	float D = (a.y + a.z) * (b.x - b.w);
	float E = (a.x + a.z) * (b.x + b.y);
	float F = (a.x - a.z) * (b.x - b.y);
	float G = (a.w + a.y) * (b.w - b.z);
	float H = (a.w - a.y) * (b.w + b.z);
	return quaternion(
		 A - ( E + F + G + H) * 0.5f,
		-C + ( E - F + G - H) * 0.5f,
		-D + ( E - F - G + H) * 0.5f,
		 B + (-E - F + G + H) * 0.5f);
#endif
}

/// Применение кватерниона к вектору.
inline float3 operator*(const float3& v, const quaternion& q)
{
#if 1
	quaternion V(v.x, v.y, v.z, 0);
	V = q * V * q.conjugate();
	//V = q.conjugate() * V * q;
	return float3(V.x, V.y, V.z);
#else
	float3 qv(q.x, q.y, q.z);
	return v + cross(qv, cross(qv, v) + v * q.w) * 2;
#endif
}

inline quaternion slerp(const quaternion& a, const quaternion& b, float t)
{
#if 1
	float d = dot(a, b);
	float4 q;
	if(d < 0)
	{
		d = -d;
		q = -b;
	}
	else
		q = b;
	if(d < 0.995f)
	{
		float angle = acos(d);
		return quaternion((a * sin(angle * (1 - t)) + q * sin(angle * t)) / sin(angle));
	}
	return quaternion(lerp<float4>(a, q, t));
#else
	// вычисление взято из btQuaternion.h

	float magnitude = sqrt(a.length2() * b.length2());

	float product = dot(a, b) / magnitude;
	if (fabs(product) != 1.0f)
	{
		// Take care of long angle case see http://en.wikipedia.org/wiki/Slerp
		const float sign = (product < 0) ? -1.0f : 1.0f;

		const float theta = acos(sign * product);
		if(fabs(theta) < 1e-8)
			throw 0;
		const float s1 = sin(sign * t * theta);
		const float d = 1.0f / sin(theta);
		const float s0 = sin((1.0f - t) * theta);

		return quaternion(
			(a.x * s0 + b.x * s1) * d,
			(a.y * s0 + b.y * s1) * d,
			(a.z * s0 + b.z * s1) * d,
			(a.w * s0 + b.w * s1) * d
			);
	}
	else
		return a;
#endif
}

END_INANITY_MATH

#endif
