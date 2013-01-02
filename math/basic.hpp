#ifndef ___INANITY_MATH_BASIC_HPP___
#define ___INANITY_MATH_BASIC_HPP___

#include <cmath>
#include "math.hpp"

BEGIN_INANITY_MATH

typedef float scalar;

inline scalar sqr(scalar s)
{
	return s * s;
}

template <typename T>
inline T clamp(T t, T a, T b)
{
	return t < a ? a : t > b ? b : t;
}

template <int n>
struct vector
{
	scalar t[n];
};

template <>
struct vector<2>
{
	union
	{
		struct
		{
			scalar x, y;
		};
		scalar t[2];
	};

	vector(scalar x = 0, scalar y = 0) : x(x), y(y) {}
};

template <>
struct vector<3>
{
	union
	{
		struct
		{
			scalar x, y, z;
		};
		scalar t[3];
	};

	vector(scalar x = 0, scalar y = 0, scalar z = 0) : x(x), y(y), z(z) {}
};

template <>
struct vector<4>
{
	union
	{
		struct
		{
			scalar x, y, z, w;
		};
		scalar t[4];
	};

	vector(scalar x = 0, scalar y = 0, scalar z = 0, scalar w = 0) : x(x), y(y), z(z), w(w) {}
};

template <int n, int m>
struct matrix
{
	scalar t[n][m];
};

template <int n>
inline void SetMatrixIdentity(matrix<n, n>& a)
{
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			a.t[i][j] = i == j;
}

template <int n, int m>
inline void SetMatrixZero(matrix<n, m>& a)
{
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < m; ++j)
			a.t[i][j] = scalar();
}

template <int n>
inline bool operator<(const vector<n>& a, const vector<n>& b)
{
	for(int i = 0; i < n; ++i)
	{
		if(a.t[i] < b.t[i]) return true;
		if(a.t[i] > b.t[i]) return false;
	}
	return false;
}

template <int n>
inline bool operator==(const vector<n>& a, const vector<n>& b)
{
	for(int i = 0; i < n; ++i)
		if(a.t[i] != b.t[i]) return false;
	return true;
}

template <int n>
inline vector<n> operator-(const vector<n>& a)
{
	vector<n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = -a.t[i];
	return r;
}

template <int n>
inline vector<n> operator+(const vector<n>& a, const vector<n>& b)
{
	vector<n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] + b.t[i];
	return r;
}

template <int n>
inline vector<n>& operator+=(vector<n>& a, const vector<n>& b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] += b.t[i];
	return a;
}

template <int n>
inline vector<n> operator-(const vector<n>& a, const vector<n>& b)
{
	vector<n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] - b.t[i];
	return r;
}

template <int n>
inline vector<n>& operator-=(vector<n>& a, const vector<n>& b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] -= b.t[i];
	return a;
}

template <int n>
inline vector<n> operator*(const vector<n>& a, const vector<n>& b)
{
	vector<n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] * b.t[i];
	return r;
}

template <int n>
inline vector<n>& operator*=(vector<n>& a, const vector<n>& b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] *= b.t[i];
	return a;
}

template <int n>
inline vector<n> operator*(const vector<n>& a, scalar b)
{
	vector<n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] * b;
	return r;
}

template <int n>
inline vector<n>& operator*=(vector<n>& a, scalar b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] *= b;
	return a;
}

template <int n>
inline vector<n> operator/(const vector<n>& a, const vector<n>& b)
{
	vector<n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] / b.t[i];
	return r;
}

template <int n>
inline vector<n>& operator/=(vector<n>& a, const vector<n>& b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] /= b.t[i];
	return a;
}

template <int n>
inline vector<n> operator/(const vector<n>& a, scalar b)
{
	vector<n> r;
	for(int i = 0; i < n; ++i)
		r.t[i] = a.t[i] / b;
	return r;
}

template <int n>
inline vector<n>& operator/=(vector<n>& a, scalar b)
{
	for(int i = 0; i < n; ++i)
		a.t[i] /= b;
	return a;
}

template <int n>
inline scalar dot(const vector<n>& a, const vector<n>& b)
{
	scalar s = scalar();
	for(int i = 0; i < n; ++i)
		s += a.t[i] * b.t[i];
	return s;
}

inline vector<3> cross(const vector<3>& a, const vector<3>& b)
{
	return vector<3>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template <int n>
inline scalar length2(const vector<n>& a)
{
	return dot(a, a);
}

template <int n>
inline scalar length(const vector<n>& a)
{
	return std::sqrt(length2(a));
}

template <int n>
inline vector<n> normalize(const vector<n>& a)
{
	return a / length(a);
}

inline vector<3> normal(const vector<3>& a, const vector<3>& b, const vector<3>& c)
{
	return cross(b - a, c - a);
}

template <typename T>
inline T lerp(const T& a, const T& b, scalar t)
{
	return a * (1 - t) + b * t;
}

template <int n, int m>
inline matrix<n, m> operator+(const matrix<n, m>& a, const matrix<n, m>& b)
{
	matrix<n, m> r;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < m; ++j)
			r.t[i][j] = a.t[i][j] + b.t[i][j];
	return r;
}

template <int n, int m>
inline matrix<n, m> operator-(const matrix<n, m>& a, const matrix<n, m>& b)
{
	matrix<n, m> r;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < m; ++j)
			r.t[i][j] = a.t[i][j] - b.t[i][j];
	return r;
}

template <int n, int m>
inline matrix<n, m> operator*(const matrix<n, m>& a, const scalar b)
{
	matrix<n, m> r;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < m; ++j)
			r.t[i][j] = a.t[i][j] * b;
	return r;
}

template <int n1, int n2, int n3>
inline matrix<n1, n3> operator*(const matrix<n1, n2>& a, const matrix<n2, n3>& b)
{
	matrix<n1, n3> r;
	for(int i = 0; i < n1; ++i)
		for(int j = 0; j < n3; ++j)
		{
			scalar s = scalar();
			for(int k = 0; k < n2; ++k)
				s += a.t[i][k] * b.t[k][j];
			r.t[i][j] = s;
		}
	return r;
}

template <int n, int m>
inline vector<m> operator*(const vector<n>& a, const matrix<n, m>& b)
{
	vector<m> r;
	for(int i = 0; i < m; ++i)
	{
		scalar s = scalar();
		for(int j = 0; j < n; ++j)
			s += a.t[j] * b.t[j][i];
		r.t[i] = s;
	}
	return r;
}

inline vector<4> operator*(const vector<3>& a, const matrix<4, 4>& b)
{
	return vector<4>(a.x, a.y, a.z, 1) * b;
}

template <int sn, int sm, int n, int m>
inline matrix<sn, sm> submatrix(const matrix<n, m>& a, int si = 0, int sj = 0)
{
	matrix<sn, sm> r;
	for(int i = 0; i < sn; ++i)
		for(int j = 0; j < sm; ++j)
			r.t[i][j] = a.t[i + si][j + sj];
	return r;
}

inline matrix<4, 4> CreateTranslationMatrix(scalar x, scalar y, scalar z)
{
	matrix<4, 4> r;
	SetMatrixIdentity(r);
	r.t[3][0] = x;
	r.t[3][1] = y;
	r.t[3][2] = z;
	return r;
}

inline matrix<4, 4> CreateScalingMatrix(scalar x, scalar y, scalar z)
{
	matrix<4, 4> r;
	SetMatrixZero(r);
	r.t[0][0] = x;
	r.t[1][1] = y;
	r.t[2][2] = z;
	r.t[3][3] = 1;
	return r;
}
inline matrix<4, 4> CreateScalingMatrix(const vector<3>& scale)
{
	return CreateScalingMatrix(scale.x, scale.y, scale.z);
}

inline matrix<4, 4> CreateRotationXMatrix(scalar alpha)
{
	scalar cosAlpha = std::cos(alpha);
	scalar sinAlpha = std::sin(alpha);
	matrix<4, 4> r;
	SetMatrixZero(r);
	r.t[0][0] = 1;
	r.t[1][1] = cosAlpha;
	r.t[1][2] = sinAlpha;
	r.t[2][1] = -sinAlpha;
	r.t[2][2] = cosAlpha;
	r.t[3][3] = 1;
	return r;
}

inline matrix<4, 4> CreateRotationZMatrix(scalar alpha)
{
	scalar cosAlpha = std::cos(alpha);
	scalar sinAlpha = std::sin(alpha);
	matrix<4, 4> r;
	SetMatrixZero(r);
	r.t[0][0] = cosAlpha;
	r.t[0][1] = sinAlpha;
	r.t[1][0] = -sinAlpha;
	r.t[1][1] = cosAlpha;
	r.t[2][2] = 1;
	r.t[3][3] = 1;
	return r;
}

inline matrix<4, 4> CreateLookAtMatrix(const vector<3>& eye, const vector<3>& target, const vector<3>& up)
{
	matrix<4, 4> r;
	SetMatrixIdentity(r);
	vector<3> z = normalize(eye - target);
	vector<3> x = normalize(cross(up, z));
	vector<3> y = cross(z, x);
	r.t[0][0] = x.x;
	r.t[0][1] = y.x;
	r.t[0][2] = z.x;
	r.t[1][0] = x.y;
	r.t[1][1] = y.y;
	r.t[1][2] = z.y;
	r.t[2][0] = x.z;
	r.t[2][1] = y.z;
	r.t[2][2] = z.z;
	r.t[3][0] = -dot(x, eye);
	r.t[3][1] = -dot(y, eye);
	r.t[3][2] = -dot(z, eye);
	return r;
}

inline matrix<4, 4> CreateProjectionPerspectiveFovMatrix(scalar fovY, scalar aspect, scalar zn, scalar zf)
{
	matrix<4, 4> r;
	SetMatrixZero(r);
	scalar yScale = 1 / tan(fovY / 2);
	scalar xScale = yScale / aspect;
	r.t[0][0] = xScale;
	r.t[1][1] = yScale;
	r.t[2][2] = zf / (zn - zf);
	r.t[3][2] = zn * zf / (zn - zf);
	r.t[2][3] = -1;
	return r;
}

typedef vector<2> float2;
typedef vector<3> float3;
typedef vector<4> float4;
typedef matrix<3, 3> float3x3;
typedef matrix<4, 4> float4x4;

END_INANITY_MATH

#endif
