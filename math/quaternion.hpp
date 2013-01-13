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
};

/// Умножение кватернионов.
/* ij = k, jk = i, ki = j
*/
inline quaternion operator*(const quaternion& a, const quaternion& b)
{
#if 0
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
	float3 qv(q.x, q.y, q.z);
	return v + cross(qv, cross(qv, v) + v * q.w) * 2;
}

inline quaternion slerp(const quaternion& a, const quaternion& b, float t)
{
	// вычисление взято из btQuaternion.h

	float magnitude = sqrt(a.length2() * b.length2()); 

	float product = dot(a, b) / magnitude;
	if (fabs(product) != 1.0f)
	{
		// Take care of long angle case see http://en.wikipedia.org/wiki/Slerp
		const float sign = (product < 0) ? -1.0f : 1.0f;

		const float theta = acos(sign * product);
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
}

END_INANITY_MATH

#endif
