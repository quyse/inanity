#ifndef ___INANITY_MATH_BASIC_HPP___
#define ___INANITY_MATH_BASIC_HPP___

#include <cmath>
#include <iostream>
#include "math.hpp"

BEGIN_INANITY_MATH

template <typename T>
inline T sqr(T t)
{
	return t * t;
}

template <typename T>
inline T clamp(T t, T a, T b)
{
	return t < a ? a : t > b ? b : t;
}

template <typename T, int n>
struct xvec
{
	T t[n];

	T& operator()(int i)
	{
		return t[i];
	}

	T operator()(int i) const
	{
		return t[i];
	}
};

template <typename T>
struct xvec<T, 2>
{
	union
	{
		struct
		{
			T x, y;
		};
		T t[2];
	};

	xvec(T x = 0, T y = 0) : x(x), y(y) {}

	T& operator()(int i)
	{
		return t[i];
	}

	T operator()(int i) const
	{
		return t[i];
	}
};

template <typename T>
struct xvec<T, 3>
{
	union
	{
		struct
		{
			T x, y, z;
		};
		T t[3];
	};

	xvec(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

	T& operator()(int i)
	{
		return t[i];
	}

	T operator()(int i) const
	{
		return t[i];
	}
};

template <typename T>
struct xvec<T, 4>
{
	union
	{
		struct
		{
			T x, y, z, w;
		};
		T t[4];
	};

	xvec(T x = 0, T y = 0, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) {}

	T& operator()(int i)
	{
		return t[i];
	}

	T operator()(int i) const
	{
		return t[i];
	}
};

template <typename T, int n>
struct generalxvec
{
	typedef xvec<T, n> Type;
};
template <typename T>
struct generalxvec<T, 1>
{
	typedef T Type;
};

template <typename T, int n, int m>
class xmat
{
private:
	// Порядок column-major.
	T t[m][n];

public:
	inline T& operator()(int i, int j)
	{
		return t[j][i];
	}

	inline T operator()(int i, int j) const
	{
		return t[j][i];
	}
};

template <typename T, int n>
inline xmat<T, n, n> identity_mat()
{
	xmat<T, n, n> a;
	for(int j = 0; j < n; ++j)
		for(int i = 0; i < n; ++i)
			a(i, j) = i == j;
	return a;
}

template <typename T, int n, int m>
inline xmat<T, n, m> zero_mat()
{
	xmat<T, n, m> a;
	for(int j = 0; j < m; ++j)
		for(int i = 0; i < n; ++i)
			a(i, j) = T();
	return a;
}

template <typename T, int n>
inline bool operator<(const xvec<T, n>& a, const xvec<T, n>& b)
{
	for(int i = 0; i < n; ++i)
	{
		if(a.t[i] < b.t[i]) return true;
		if(a.t[i] > b.t[i]) return false;
	}
	return false;
}

template <typename T, int n>
inline bool operator==(const xvec<T, n>& a, const xvec<T, n>& b)
{
	for(int i = 0; i < n; ++i)
		if(a.t[i] != b.t[i]) return false;
	return true;
}

template <typename T, int n>
inline xvec<T, n> operator-(const xvec<T, n>& a)
{
	xvec<T, n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = -a.t[i];
	return r;
}

template <typename T, int n>
inline xvec<T, n> operator+(const xvec<T, n>& a, const xvec<T, n>& b)
{
	xvec<T, n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] + b.t[i];
	return r;
}

template <typename T, int n>
inline xvec<T, n>& operator+=(xvec<T, n>& a, const xvec<T, n>& b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] += b.t[i];
	return a;
}

template <typename T, int n>
inline xvec<T, n> operator-(const xvec<T, n>& a, const xvec<T, n>& b)
{
	xvec<T, n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] - b.t[i];
	return r;
}

template <typename T, int n>
inline xvec<T, n>& operator-=(xvec<T, n>& a, const xvec<T, n>& b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] -= b.t[i];
	return a;
}

template <typename T, int n>
inline xvec<T, n> operator*(const xvec<T, n>& a, const xvec<T, n>& b)
{
	xvec<T, n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] * b.t[i];
	return r;
}

template <typename T, int n>
inline xvec<T, n>& operator*=(xvec<T, n>& a, const xvec<T, n>& b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] *= b.t[i];
	return a;
}

template <typename T, int n>
inline xvec<T, n> operator*(const xvec<T, n>& a, T b)
{
	xvec<T, n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] * b;
	return r;
}

template <typename T, int n>
inline xvec<T, n>& operator*=(xvec<T, n>& a, T b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] *= b;
	return a;
}

template <typename T, int n>
inline xvec<T, n> operator/(const xvec<T, n>& a, const xvec<T, n>& b)
{
	xvec<T, n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] / b.t[i];
	return r;
}

template <typename T, int n>
inline xvec<T, n>& operator/=(xvec<T, n>& a, const xvec<T, n>& b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] /= b.t[i];
	return a;
}

template <typename T, int n>
inline xvec<T, n> operator/(const xvec<T, n>& a, T b)
{
	xvec<T, n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] / b;
	return r;
}

template <typename T, int n>
inline xvec<T, n>& operator/=(xvec<T, n>& a, T b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] /= b;
	return a;
}

template <typename T, int n>
inline T dot(const xvec<T, n>& a, const xvec<T, n>& b)
{
	T s = T();
	for(int i = 0; i < n; ++i)
		s += a.t[i] * b.t[i];
	return s;
}

template <typename T>
inline xvec<T, 3> cross(const xvec<T, 3>& a, const xvec<T, 3>& b)
{
	return xvec<T, 3>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template <typename T, int n>
inline T length2(const xvec<T, n>& a)
{
	return dot(a, a);
}

template <typename T, int n>
inline T length(const xvec<T, n>& a)
{
	return std::sqrt(length2(a));
}

template <typename T, int n>
inline xvec<T, n> normalize(const xvec<T, n>& a)
{
	return a / length(a);
}

template <typename T>
inline xvec<T, 3> normal(const xvec<T, 3>& a, const xvec<T, 3>& b, const xvec<T, 3>& c)
{
	return cross(b - a, c - a);
}

template <typename V, typename T>
inline V lerp(const V& a, const V& b, T t)
{
	return a * (1 - t) + b * t;
}

template <typename T, int n>
inline std::ostream& operator<<(std::ostream& a, const xvec<T, n>& b)
{
	for(int i = 0; i < n; ++i)
	{
		if(i)
			a << ' ';
		a << b(i);
	}
	return a;
}

template <typename T, int n, int m>
inline std::ostream& operator<<(std::ostream& a, const xmat<T, n, m>& b)
{
	for(int i = 0; i < n; ++i)
	{
		for(int j = 0; j < m; ++j)
		{
			if(j)
				a << ' ';
			a << b(i, j);
		}
		a << '\n';
	}
	return a;
}

template <typename T, int n, int m>
inline xmat<T, n, m> operator+(const xmat<T, n, m>& a, const xmat<T, n, m>& b)
{
	xmat<T, n, m> r;
	for(int j = 0; j < m; ++j)
		for(int i = 0; i < n; ++i)
			r(i, j) = a(i, j) + b(i, j);
	return r;
}

template <typename T, int n, int m>
inline xmat<T, n, m> operator-(const xmat<T, n, m>& a, const xmat<T, n, m>& b)
{
	xmat<T, n, m> r;
	for(int j = 0; j < m; ++j)
		for(int i = 0; i < n; ++i)
			r(i, j) = a(i, j) - b(i, j);
	return r;
}

template <typename T, int n, int m>
inline xmat<T, n, m> operator*(const xmat<T, n, m>& a, const T b)
{
	xmat<T, n, m> r;
	for(int j = 0; j < m; ++j)
		for(int i = 0; i < n; ++i)
			r(i, j) = a(i, j) * b;
	return r;
}

template <typename T, int n1, int n2, int n3>
inline xmat<T, n1, n3> operator*(const xmat<T, n1, n2>& a, const xmat<T, n2, n3>& b)
{
	xmat<T, n1, n3> r;
	for(int j = 0; j < n3; ++j)
		for(int i = 0; i < n1; ++i)
		{
			T s = T();
			for(int k = 0; k < n2; ++k)
				s += a(i, k) * b(k, j);
			r(i, j) = s;
		}
	return r;
}

template <typename T, int n, int m>
inline xvec<T, m> operator*(const xvec<T, n>& a, const xmat<T, n, m>& b)
{
	xvec<T, m> r;
	for(int j = 0; j < m; ++j)
	{
		T s = T();
		for(int i = 0; i < n; ++i)
			s += a(i) * b(i, j);
		r(j) = s;
	}
	return r;
}

template <typename T, int n, int m>
inline xvec<T, n> operator*(const xmat<T, n, m>& a, const xvec<T, m>& b)
{
	xvec<T, n> r;
	for(int i = 0; i < n; ++i)
	{
		T s = T();
		for(int j = 0; j < m; ++j)
			s += a(i, j) * b(j);
		r(i) = s;
	}
	return r;
}

template <typename T>
inline xvec<T, 4> operator*(const xvec<T, 3>& a, const xmat<T, 4, 4>& b)
{
	return xvec<T, 4>(a.x, a.y, a.z, 1) * b;
}

template <typename T>
inline xvec<T, 4> operator*(const xmat<T, 4, 4>& a, const xvec<T, 3>& b)
{
	return a * xvec<T, 4>(b.x, b.y, b.z, 1);
}

template <typename T, int sn, int sm, int n, int m>
inline xmat<T, sn, sm> submat(const xmat<T, n, m>& a, int si = 0, int sj = 0)
{
	xmat<T, sn, sm> r;
	for(int j = 0; j < sm; ++j)
		for(int i = 0; i < sn; ++i)
			r(i, j) = a(i + si, j + sj);
	return r;
}

//******* Typedef'ы для удобства.

//*** float
typedef xvec<float, 2> vec2;
typedef xvec<float, 3> vec3;
typedef xvec<float, 4> vec4;
typedef xmat<float, 1, 2> mat1x2;
typedef xmat<float, 1, 3> mat1x3;
typedef xmat<float, 1, 4> mat1x4;
typedef xmat<float, 2, 1> mat2x1;
typedef xmat<float, 2, 2> mat2x2;
typedef xmat<float, 2, 3> mat2x3;
typedef xmat<float, 2, 4> mat2x4;
typedef xmat<float, 3, 1> mat3x1;
typedef xmat<float, 3, 2> mat3x2;
typedef xmat<float, 3, 3> mat3x3;
typedef xmat<float, 3, 4> mat3x4;
typedef xmat<float, 4, 1> mat4x1;
typedef xmat<float, 4, 2> mat4x2;
typedef xmat<float, 4, 3> mat4x3;
typedef xmat<float, 4, 4> mat4x4;

//*** double
typedef xvec<double, 2> dvec2;
typedef xvec<double, 3> dvec3;
typedef xvec<double, 4> dvec4;
typedef xmat<double, 1, 2> dmat1x2;
typedef xmat<double, 1, 3> dmat1x3;
typedef xmat<double, 1, 4> dmat1x4;
typedef xmat<double, 2, 1> dmat2x1;
typedef xmat<double, 2, 2> dmat2x2;
typedef xmat<double, 2, 3> dmat2x3;
typedef xmat<double, 2, 4> dmat2x4;
typedef xmat<double, 3, 1> dmat3x1;
typedef xmat<double, 3, 2> dmat3x2;
typedef xmat<double, 3, 3> dmat3x3;
typedef xmat<double, 3, 4> dmat3x4;
typedef xmat<double, 4, 1> dmat4x1;
typedef xmat<double, 4, 2> dmat4x2;
typedef xmat<double, 4, 3> dmat4x3;
typedef xmat<double, 4, 4> dmat4x4;

//*** int
typedef xvec<int, 2> ivec2;
typedef xvec<int, 3> ivec3;
typedef xvec<int, 4> ivec4;
typedef xmat<int, 1, 2> imat1x2;
typedef xmat<int, 1, 3> imat1x3;
typedef xmat<int, 1, 4> imat1x4;
typedef xmat<int, 2, 1> imat2x1;
typedef xmat<int, 2, 2> imat2x2;
typedef xmat<int, 2, 3> imat2x3;
typedef xmat<int, 2, 4> imat2x4;
typedef xmat<int, 3, 1> imat3x1;
typedef xmat<int, 3, 2> imat3x2;
typedef xmat<int, 3, 3> imat3x3;
typedef xmat<int, 3, 4> imat3x4;
typedef xmat<int, 4, 1> imat4x1;
typedef xmat<int, 4, 2> imat4x2;
typedef xmat<int, 4, 3> imat4x3;
typedef xmat<int, 4, 4> imat4x4;

//*** uint
typedef unsigned int uint;
typedef xvec<unsigned int, 2> uvec2;
typedef xvec<unsigned int, 3> uvec3;
typedef xvec<unsigned int, 4> uvec4;
typedef xmat<unsigned int, 1, 2> umat1x2;
typedef xmat<unsigned int, 1, 3> umat1x3;
typedef xmat<unsigned int, 1, 4> umat1x4;
typedef xmat<unsigned int, 2, 1> umat2x1;
typedef xmat<unsigned int, 2, 2> umat2x2;
typedef xmat<unsigned int, 2, 3> umat2x3;
typedef xmat<unsigned int, 2, 4> umat2x4;
typedef xmat<unsigned int, 3, 1> umat3x1;
typedef xmat<unsigned int, 3, 2> umat3x2;
typedef xmat<unsigned int, 3, 3> umat3x3;
typedef xmat<unsigned int, 3, 4> umat3x4;
typedef xmat<unsigned int, 4, 1> umat4x1;
typedef xmat<unsigned int, 4, 2> umat4x2;
typedef xmat<unsigned int, 4, 3> umat4x3;
typedef xmat<unsigned int, 4, 4> umat4x4;

//*** quaternion
typedef vec4 quat;
typedef dvec4 dquat;

END_INANITY_MATH

#endif
