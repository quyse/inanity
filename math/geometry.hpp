#ifndef ___INANITY_MATH_GEOMETRY_HPP___
#define ___INANITY_MATH_GEOMETRY_HPP___

#include "basic.hpp"

BEGIN_INANITY_MATH

template <typename T>
xmat<T, 4, 4> CreateTranslationMatrix(const xvec<T, 3>& translation)
{
	xmat<T, 4, 4> r = identity_mat<T, 4>();
	r(0, 3) = translation(0);
	r(1, 3) = translation(1);
	r(2, 3) = translation(2);
	return r;
}

template <typename T>
xmat<T, 4, 4> CreateLookAtMatrix(const xvec<T, 3>& eye, const xvec<T, 3>& target, const xvec<T, 3>& up)
{
	xmat<T, 4, 4> r;
	xvec<T, 3> z = normalize(eye - target);
	xvec<T, 3> x = normalize(cross(up, z));
	xvec<T, 3> y = cross(z, x);
	r(0, 0) = x.x;
	r(1, 0) = y.x;
	r(2, 0) = z.x;
	r(3, 0) = 0;
	r(0, 1) = x.y;
	r(1, 1) = y.y;
	r(2, 1) = z.y;
	r(3, 1) = 0;
	r(0, 2) = x.z;
	r(1, 2) = y.z;
	r(2, 2) = z.z;
	r(3, 2) = 0;
	r(0, 3) = -dot(x, eye);
	r(1, 3) = -dot(y, eye);
	r(2, 3) = -dot(z, eye);
	r(3, 3) = 1;
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
	r(2, 3) = zn * zf / (zn - zf);
	r(3, 2) = -1;
	return r;
}

template <typename T>
xvec<T, 4> axis_rotation(const xvec<T, 3>& axis, T angle)
{
	angle /= 2;
	T angleSin = sin(angle);
	T angleCos = cos(angle);
	return xvec<T, 4>(
		axis.x * angleSin,
		axis.y * angleSin,
		axis.z * angleSin,
		angleCos);
}

END_INANITY_MATH

#endif
