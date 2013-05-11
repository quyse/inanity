#ifndef ___INANITY_MATH_GEOMETRY_HPP___
#define ___INANITY_MATH_GEOMETRY_HPP___

#include "basic.hpp"

BEGIN_INANITY_MATH

template <typename T>
xmat<T, 4, 4> CreateTranslationMatrix(T x, T y, T z)
{
	xmat<T, 4, 4> r = identity_mat<T, 4>();
	r(3, 0) = x;
	r(3, 1) = y;
	r(3, 2) = z;
	return r;
}
template <typename T>
xmat<T, 4, 4> CreateTranslationMatrix(const xvec<T, 3>& translation)
{
	return CreateTranslationMatrix(translation.x, translation.y, translation.z);
}

template <typename T>
xmat<T, 4, 4> CreateScalingMatrix(T x, T y, T z)
{
	xmat<T, 4, 4> r = zero_mat<T, 4, 4>();
	r(0, 0) = x;
	r(1, 1) = y;
	r(2, 2) = z;
	r(3, 3) = 1;
	return r;
}
template <typename T>
xmat<T, 4, 4> CreateScalingMatrix(const xvec<T, 3>& scale)
{
	return CreateScalingMatrix<T>(scale.x, scale.y, scale.z);
}

template <typename T>
xmat<T, 4, 4> CreateRotationXMatrix(T alpha)
{
	T cosAlpha = std::cos(alpha);
	T sinAlpha = std::sin(alpha);
	xmat<T, 4, 4> r = zero_mat<T, 4, 4>();
	r(0, 0) = 1;
	r(1, 1) = cosAlpha;
	r(1, 2) = sinAlpha;
	r(2, 1) = -sinAlpha;
	r(2, 2) = cosAlpha;
	r(3, 3) = 1;
	return r;
}

template <typename T>
xmat<T, 4, 4> CreateRotationZMatrix(T alpha)
{
	T cosAlpha = std::cos(alpha);
	T sinAlpha = std::sin(alpha);
	xmat<T, 4, 4> r = zero_mat<T, 4, 4>();
	r(0, 0) = cosAlpha;
	r(0, 1) = sinAlpha;
	r(1, 0) = -sinAlpha;
	r(1, 1) = cosAlpha;
	r(2, 2) = 1;
	r(3, 3) = 1;
	return r;
}

template <typename T>
xmat<T, 4, 4> CreateLookAtMatrix(const xvec<T, 3>& eye, const xvec<T, 3>& target, const xvec<T, 3>& up)
{
	xmat<T, 4, 4> r = identity_mat<T, 4>();
	xvec<T, 3> z = normalize(eye - target);
	xvec<T, 3> x = normalize(cross(up, z));
	xvec<T, 3> y = cross(z, x);
	r(0, 0) = x.x;
	r(0, 1) = y.x;
	r(0, 2) = z.x;
	r(1, 0) = x.y;
	r(1, 1) = y.y;
	r(1, 2) = z.y;
	r(2, 0) = x.z;
	r(2, 1) = y.z;
	r(2, 2) = z.z;
	r(3, 0) = -dot(x, eye);
	r(3, 1) = -dot(y, eye);
	r(3, 2) = -dot(z, eye);
	return r;
}

template <typename T>
xmat<T, 4, 4> CreateProjectionPerspectiveFovMatrix(T fovY, T aspect, T zn, T zf)
{
	xmat<T, 4, 4> r = zero_mat<T, 4, 4>();
	T yScale = 1 / tan(fovY / 2);
	T xScale = yScale / aspect;
	r(0, 0) = xScale;
	r(1, 1) = yScale;
	r(2, 2) = zf / (zn - zf);
	r(3, 2) = zn * zf / (zn - zf);
	r(2, 3) = -1;
	return r;
}

END_INANITY_MATH

#endif
