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
template <typename T>
struct xquat : xvec<T, 4>
{
	/// Конструктор по умолчанию - кватернион без поворота.
	inline xquat() : xvec<T, 4>(0, 0, 0, 1) {}

	/// Конструктор из параметров.
	inline xquat(T x, T y, T z, T w)
	: xvec<T, 4>(x, y, z, w) {}

	/// Конструктор из оси и угла.
	inline xquat(const xvec<T, 3>& axis, T angle)
	{
		angle /= 2;
		T angleSin = sin(angle);
		T angleCos = cos(angle);
		x = axis.x * angleSin;
		y = axis.y * angleSin;
		z = axis.z * angleSin;
		w = angleCos;
	}

	/// Явный конструктор из вектора
	inline explicit xquat(const xvec<T, 4>& v)
	: xvec<T, 4>(v) {}

	/// Оператор присваивания.
	inline xquat& operator=(const xvec<T, 4>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	/// Инверсия.
	inline xquat conjugate() const
	{
		return xquat(-x, -y, -z, w);
	}

	/// Квадрат длины кватерниона.
	inline T length2() const
	{
		return x * x + y * y + z * z + w * w;
	}

	/// Длина кватерниона.
	inline T length() const
	{
		return std::sqrt(length2());
	}

	/// Нормализация кватерниона.
	inline xquat normalize() const
	{
		T invLength = 1 / length();
		return xquat(x * invLength, y * invLength, z * invLength, w * invLength);
	}

	/// Матрица поворота.
	operator xmat<T, 4, 4>() const
	{
		xmat<T, 4, 4> r;
		r(0, 0) = 1 - (y * y + z * z) * 2;
		r(0, 1) = (x * y - w * z) * 2;
		r(0, 2) = (x * z + w * y) * 2;
		r(0, 3) = 0;
		r(1, 0) = (x * y + w * z) * 2;
		r(1, 1) = 1 - (x * x + z * z) * 2;
		r(1, 2) = (y * z - w * x) * 2;
		r(1, 3) = 0;
		r(2, 0) = (x * z - w * y) * 2;
		r(2, 1) = (y * z + w * x) * 2;
		r(2, 2) = 1 - (x * x + y * y) * 2;
		r(2, 3) = 0;
		r(3, 0) = 0;
		r(3, 1) = 0;
		r(3, 2) = 0;
		r(3, 3) = 1;
		return r;
	}
};

/// Умножение кватернионов.
/* ij = k, jk = i, ki = j
*/
template <typename T>
inline xquat<T> operator*(const xquat<T>& a, const xquat<T>& b)
{
#if 1
	return xquat<T>(
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
	return xquat(
		 A - ( E + F + G + H) * 0.5f,
		-C + ( E - F + G - H) * 0.5f,
		-D + ( E - F - G + H) * 0.5f,
		 B + (-E - F + G + H) * 0.5f);
#endif
}

/// Применение кватерниона к вектору.
template <typename T>
inline xvec<T, 3> operator*(const xvec<T, 3>& v, const xquat<T>& q)
{
#if 1
	xquat<T> V(v.x, v.y, v.z, 0);
	V = q * V * q.conjugate();
	//V = q.conjugate() * V * q;
	return xvec<T, 3>(V.x, V.y, V.z);
#else
	vec3 qv(q.x, q.y, q.z);
	return v + cross(qv, cross(qv, v) + v * q.w) * 2;
#endif
}

template <typename T>
inline xquat<T> slerp(const xquat<T>& a, const xquat<T>& b, T t)
{
#if 1
	T d = dot(a, b);
	xvec<T, 4> q;
	if(d < 0)
	{
		d = -d;
		q = -b;
	}
	else
		q = b;
	if(d < (T)0.995f)
	{
		T angle = acos(d);
		return xquat<T>((a * sin(angle * (1 - t)) + q * sin(angle * t)) / sin(angle));
	}
	return xquat<T>(lerp<xvec<T, 4> >(a, q, t));
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

		return xquat(
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

//*** typedef'ы для удобства

typedef xquat<float> quat;
typedef xquat<double> dquat;

END_INANITY_MATH

#endif
