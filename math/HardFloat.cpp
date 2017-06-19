#include "HardFloat.hpp"
#include <xmmintrin.h>
#include <cassert>

// Debug checks
// checks performed via softfloat
#if defined(_DEBUG) && 0
#include "../deps/softfloat/softfloat.hpp"
#define CHECKED_IMPL_BEGIN(x, q) \
	{ \
		SoftFloat::Float theirs = q(SoftFloat::Float(x.f)); \
		HardFloat ours = ([x]() mutable
#define CHECKED_IMPL_BEGIN2(a, b, q) \
	{ \
		SoftFloat::Float theirs = q(SoftFloat::Float(a.f), SoftFloat::Float(b.f)); \
		HardFloat ours = ([a, b]() mutable
#define CHECKED_IMPL_BEGIN_OP2(a, b, op) \
	{ \
		SoftFloat::Float theirs = SoftFloat::Float(a.f) op SoftFloat::Float(b.f); \
		HardFloat ours = ([a, b]() mutable
#define CHECKED_IMPL_END() \
	)(); \
		assert(theirs.getInternalUint32() == ours.getInternalUint32()); \
		return ours; \
	}
#define CHECKED_IMPL_BEGIN_OP0(op) \
	{ \
		op theirs = (op)SoftFloat::Float(f); \
		op ours = ([=]()
#define CHECKED_IMPL_END_OP0() \
	)(); \
		assert(theirs == ours); \
		return ours; \
	}
#define CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, op) \
	{ \
		bool theirs = SoftFloat::Float(a.f) op SoftFloat::Float(b.f); \
		bool ours = ([a, b]() mutable
#else
#define CHECKED_IMPL_BEGIN(x, f)
#define CHECKED_IMPL_BEGIN2(a, b, f)
#define CHECKED_IMPL_BEGIN_OP2(a, b, op)
#define CHECKED_IMPL_END()
#define CHECKED_IMPL_BEGIN_OP0(op)
#define CHECKED_IMPL_END_OP0()
#define CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, op)
#endif

BEGIN_INANITY_MATH

const HardFloat HardFloat::pi = HardFloat::fromUint32Const(0x40490fdb);
const HardFloat HardFloat::pi2 = HardFloat::fromUint32Const(0x40c90fdb);
const HardFloat HardFloat::pi_2 = HardFloat::fromUint32Const(0x3fc90fdb);

HardFloat::HardFloat() : f(0) {}

HardFloat::HardFloat(int32_t n)
{
	_mm_store_ss(&f, _mm_cvt_si2ss(_mm_setzero_ps(), (int)n));
}

HardFloat::HardFloat(uint32_t n)
{
	_mm_store_ss(&f, _mm_cvt_si2ss(_mm_setzero_ps(), (int)n));
}

HardFloat::HardFloat(float n) : f(n) {}

HardFloat HardFloat::operator-() const
{
	float q = f;
	*(uint32_t*)&q ^= 0x80000000U;
	return HardFloat(q);
}

HardFloat operator+(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2(a, b, +)
{
	return HardFloat(_mm_cvtss_f32(_mm_add_ss(_mm_set_ss(a.f), _mm_set_ss(b.f))));
} CHECKED_IMPL_END()

HardFloat operator-(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2(a, b, -)
{
	return HardFloat(_mm_cvtss_f32(_mm_sub_ss(_mm_set_ss(a.f), _mm_set_ss(b.f))));
} CHECKED_IMPL_END()

HardFloat operator*(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2(a, b, *)
{
	return HardFloat(_mm_cvtss_f32(_mm_mul_ss(_mm_set_ss(a.f), _mm_set_ss(b.f))));
} CHECKED_IMPL_END()

HardFloat operator/(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2(a, b, /)
{
	return HardFloat(_mm_cvtss_f32(_mm_div_ss(_mm_set_ss(a.f), _mm_set_ss(b.f))));
} CHECKED_IMPL_END()

HardFloat fmod(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN2(a, b, fmod)
{
	__m128 p = _mm_set_ss(a.f);
	__m128 q = _mm_set_ss(b.f);
	return HardFloat(_mm_cvtss_f32(_mm_sub_ss(p, _mm_mul_ss(_mm_cvt_si2ss(p, _mm_cvtt_ss2si(_mm_div_ss(p, q))), q))));
} CHECKED_IMPL_END()

HardFloat& HardFloat::operator+=(HardFloat b)
{
	_mm_store_ss(&f, _mm_add_ss(_mm_set_ss(f), _mm_set_ss(b.f)));
	return *this;
}

HardFloat& HardFloat::operator-=(HardFloat b)
{
	_mm_store_ss(&f, _mm_sub_ss(_mm_set_ss(f), _mm_set_ss(b.f)));
	return *this;
}

HardFloat& HardFloat::operator*=(HardFloat b)
{
	_mm_store_ss(&f, _mm_mul_ss(_mm_set_ss(f), _mm_set_ss(b.f)));
	return *this;
}

HardFloat& HardFloat::operator/=(HardFloat b)
{
	_mm_store_ss(&f, _mm_div_ss(_mm_set_ss(f), _mm_set_ss(b.f)));
	return *this;
}

bool operator==(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, ==)
{
	return !!_mm_comieq_ss(_mm_set_ss(a.f), _mm_set_ss(b.f));
} CHECKED_IMPL_END_OP0()

bool operator!=(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, !=)
{
	return !!_mm_comineq_ss(_mm_set_ss(a.f), _mm_set_ss(b.f));
} CHECKED_IMPL_END_OP0()

bool operator<(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, <)
{
	return !!_mm_comilt_ss(_mm_set_ss(a.f), _mm_set_ss(b.f));
} CHECKED_IMPL_END_OP0()

bool operator<=(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, <=)
{
	return !!_mm_comile_ss(_mm_set_ss(a.f), _mm_set_ss(b.f));
} CHECKED_IMPL_END_OP0()

bool operator>(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, >)
{
	return !!_mm_comigt_ss(_mm_set_ss(a.f), _mm_set_ss(b.f));
} CHECKED_IMPL_END_OP0()

bool operator>=(HardFloat a, HardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, >=)
{
	return !!_mm_comige_ss(_mm_set_ss(a.f), _mm_set_ss(b.f));
} CHECKED_IMPL_END_OP0()

HardFloat abs(HardFloat a) CHECKED_IMPL_BEGIN(a, abs)
{
	float f = a.f;
	*(uint32_t*)&f &= 0x7FFFFFFFU;
	return HardFloat(f);
} CHECKED_IMPL_END()

HardFloat floor(HardFloat a) CHECKED_IMPL_BEGIN(a, floor)
{
	__m128 p = _mm_set_ss(a.f);
	__m128 t = _mm_cvtepi32_ps(_mm_cvttps_epi32(p));
	return HardFloat(_mm_cvtss_f32(_mm_sub_ps(t, _mm_and_ps(_mm_cmplt_ps(p, t), _mm_set_ss(1.0f)))));
} CHECKED_IMPL_END()

HardFloat ceil(HardFloat a) CHECKED_IMPL_BEGIN(a, ceil)
{
	__m128 p = _mm_set_ss(a.f);
	__m128 t = _mm_cvtepi32_ps(_mm_cvttps_epi32(p));
	return HardFloat(_mm_cvtss_f32(_mm_add_ps(t, _mm_and_ps(_mm_cmpgt_ps(p, t), _mm_set_ss(1.0f)))));
} CHECKED_IMPL_END()

HardFloat trunc(HardFloat a) CHECKED_IMPL_BEGIN(a, trunc)
{
	return HardFloat(_mm_cvtss_f32(_mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_set_ss(a.f)))));
} CHECKED_IMPL_END()

HardFloat sqrt(HardFloat a) CHECKED_IMPL_BEGIN(a, sqrt)
{
	return HardFloat(_mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(a.f))));
} CHECKED_IMPL_END()

HardFloat sin(HardFloat a) CHECKED_IMPL_BEGIN(a, sin)
{
	// handle negative values
	if(a < HardFloat()) return -sin(-a);

	// reduce to range [0, pi]
	HardFloat n;
	if(a > HardFloat::pi)
	{
		n = trunc(a / HardFloat::pi);
		a -= n * HardFloat::pi;
	}
	// reduce to range [0, pi/2]
	if(a > HardFloat::pi_2) a = HardFloat::pi - a;

	static const HardFloat
		k1 = HardFloat::fromUint32Const(0x3f7ff052), //  0.99976073735983227f
		k3 = HardFloat::fromUint32Const(0xbe29c7cc), // -0.16580121984779175f
		k5 = HardFloat::fromUint32Const(0x3bf7d14a); //  0.00756279111686865f

	HardFloat a2 = a * a, a3 = a2 * a, a5 = a3 * a2;

	a = a * k1 + a3 * k3 + a5 * k5;

	return ((int)n % 2 == 0) ? a : -a;
} CHECKED_IMPL_END()

HardFloat cos(HardFloat a) CHECKED_IMPL_BEGIN(a, cos)
{
	return sin(a + HardFloat::pi_2);
} CHECKED_IMPL_END()

HardFloat atan(HardFloat a) CHECKED_IMPL_BEGIN(a, atan)
{
	// handle negative values
	if(a < HardFloat()) return -atan(-a);

	if(a > HardFloat(1.0f)) return HardFloat::pi_2 - atan(HardFloat(1.0f) / a);

	static const HardFloat
		k1 = HardFloat::fromUint32Const(0x3f7ff50e), //  0.9998329769337240f
		k3 = HardFloat::fromUint32Const(0xbea75d21), // -0.3268824051434949f
		k5 = HardFloat::fromUint32Const(0x3e22d89e), //  0.1590294514698240f
		k7 = HardFloat::fromUint32Const(0xbd407012); // -0.0469818803609288f

	HardFloat a2 = a * a, a4 = a2 * a2, a6 = a4 * a2;

	return a * (k1 + a2 * k3 + a4 * k5 + a6 * k7);
} CHECKED_IMPL_END()

HardFloat atan2(HardFloat y, HardFloat x) CHECKED_IMPL_BEGIN2(y, x, atan2)
{
	bool xs = x >= HardFloat();
	if(y == HardFloat()) return xs ? HardFloat() : HardFloat::pi;

	bool ys = y >= HardFloat();
	if(x == HardFloat()) return ys ? HardFloat::pi_2 : (-HardFloat::pi_2);

	HardFloat a = atan(y / x);
	return (xs == ys) ? (xs ? a : (a - HardFloat::pi)) : (xs ? a : (a + HardFloat::pi));
} CHECKED_IMPL_END()

HardFloat::operator int32_t() const CHECKED_IMPL_BEGIN_OP0(int32_t)
{
	return (int32_t)_mm_cvtt_ss2si(_mm_set_ss(f));
} CHECKED_IMPL_END_OP0()

HardFloat::operator uint32_t() const CHECKED_IMPL_BEGIN_OP0(uint32_t)
{
	return (uint32_t)_mm_cvtt_ss2si(_mm_set_ss(f));
} CHECKED_IMPL_END_OP0()

HardFloat::operator float() const CHECKED_IMPL_BEGIN_OP0(float)
{
	return f;
} CHECKED_IMPL_END_OP0()

uint32_t HardFloat::getInternalUint32() const
{
	return *(uint32_t*)&f;
}

HardFloat HardFloat::fromUint32Const(uint32_t a)
{
	HardFloat f;
	*(uint32_t*)&f.f = a;
	return f;
}

END_INANITY_MATH

namespace std
{
	Inanity::Math::HardFloat numeric_limits<Inanity::Math::HardFloat>::min()
	{
		return Inanity::Math::HardFloat(numeric_limits<float>::min());
	}

	Inanity::Math::HardFloat numeric_limits<Inanity::Math::HardFloat>::lowest()
	{
		return Inanity::Math::HardFloat(numeric_limits<float>::lowest());
	}

	Inanity::Math::HardFloat numeric_limits<Inanity::Math::HardFloat>::max()
	{
		return Inanity::Math::HardFloat(numeric_limits<float>::max());
	}
}
