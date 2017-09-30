#include "EmsHardFloat.hpp"
#include <cmath>
#include <cassert>

// Debug checks
// checks performed via softfloat
#if defined(_DEBUG) && 0
#include "../deps/softfloat/softfloat.hpp"
#define CHECKED_IMPL_BEGIN(x, q) \
	{ \
		EmsHardFloat A = x, B; \
		SoftFloat::Float theirs = q(SoftFloat::Float(x.f)); \
		EmsHardFloat ours = ([x]() mutable
#define CHECKED_IMPL_BEGIN2(a, b, q) \
	{ \
		EmsHardFloat A = a, B = b; \
		SoftFloat::Float theirs = q(SoftFloat::Float(a.f), SoftFloat::Float(b.f)); \
		EmsHardFloat ours = ([a, b]() mutable
#define CHECKED_IMPL_BEGIN_OP2(a, b, op) \
	{ \
		EmsHardFloat A = a, B = b; \
		SoftFloat::Float theirs = SoftFloat::Float(a.f) op SoftFloat::Float(b.f); \
		EmsHardFloat ours = ([a, b]() mutable
#define CHECKED_IMPL_END(msg) \
	)(); \
		if(theirs.getInternalUint32() != ours.d) { printf("CHECKED_IMPL_END: %s %f %f = (ours) %f (theirs) %f\n", msg, A.f, B.f, ours.f, (float)theirs); } \
		assert(((void)msg, theirs.getInternalUint32() == ours.d)); \
		return ours; \
	}
#define CHECKED_IMPL_BEGIN_OP0(op) \
	{ \
		EmsHardFloat A = *this, B; \
		op theirs = (op)SoftFloat::Float(f); \
		op ours = ([=]()
#define CHECKED_IMPL_END_OP0(msg) \
	)(); \
		if(theirs != ours) { printf("CHECKED_IMPL_END: %s %f %f\n", msg, A.f, B.f); } \
		assert(((void)msg, theirs == ours)); \
		return ours; \
	}
#define CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, op) \
	{ \
		EmsHardFloat A = a, B = b; \
		bool theirs = SoftFloat::Float(a.f) op SoftFloat::Float(b.f); \
		bool ours = ([a, b]() mutable
#else
#define CHECKED_IMPL_BEGIN(x, f)
#define CHECKED_IMPL_BEGIN2(a, b, f)
#define CHECKED_IMPL_BEGIN_OP2(a, b, op)
#define CHECKED_IMPL_END(msg)
#define CHECKED_IMPL_BEGIN_OP0(op)
#define CHECKED_IMPL_END_OP0(msg)
#define CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, op)
#endif

BEGIN_INANITY_MATH

const EmsHardFloat EmsHardFloat::pi = EmsHardFloat::fromUint32Const(0x40490fdb);
const EmsHardFloat EmsHardFloat::pi2 = EmsHardFloat::fromUint32Const(0x40c90fdb);
const EmsHardFloat EmsHardFloat::pi_2 = EmsHardFloat::fromUint32Const(0x3fc90fdb);

EmsHardFloat::EmsHardFloat() : f(0)
{
}

EmsHardFloat::EmsHardFloat(int32_t n) : f(n)
{
}

EmsHardFloat::EmsHardFloat(uint32_t n) : f(n)
{
}

EmsHardFloat::EmsHardFloat(float n) : f(n) {}

EmsHardFloat EmsHardFloat::operator-() const
{
	return EmsHardFloat(-f);
}

EmsHardFloat operator+(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2(a, b, +)
{
	return EmsHardFloat(a.f + b.f);
} CHECKED_IMPL_END("+")

EmsHardFloat operator-(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2(a, b, -)
{
	return EmsHardFloat(a.f - b.f);
} CHECKED_IMPL_END("-")

EmsHardFloat operator*(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2(a, b, *)
{
	return EmsHardFloat(a.f * b.f);
} CHECKED_IMPL_END("*")

EmsHardFloat operator/(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2(a, b, /)
{
	return EmsHardFloat(a.f / b.f);
} CHECKED_IMPL_END("/")

EmsHardFloat fmod(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN2(a, b, fmod)
{
	// return EmsHardFloat(std::fmod(a.f, b.f));
	return EmsHardFloat(a.f - trunc(a.f / b.f) * b.f);
} CHECKED_IMPL_END("fmod")

EmsHardFloat& EmsHardFloat::operator+=(EmsHardFloat b)
{
	f += b.f;
	return *this;
}

EmsHardFloat& EmsHardFloat::operator-=(EmsHardFloat b)
{
	f -= b.f;
	return *this;
}

EmsHardFloat& EmsHardFloat::operator*=(EmsHardFloat b)
{
	f *= b.f;
	return *this;
}

EmsHardFloat& EmsHardFloat::operator/=(EmsHardFloat b)
{
	f /= b.f;
	return *this;
}

bool operator==(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, ==)
{
	return a.f == b.f;
} CHECKED_IMPL_END_OP0("==")

bool operator!=(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, !=)
{
	return a.f != b.f;
} CHECKED_IMPL_END_OP0("!=")

bool operator<(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, <)
{
	return a.f < b.f;
} CHECKED_IMPL_END_OP0("<")

bool operator<=(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, <=)
{
	return a.f <= b.f;
} CHECKED_IMPL_END_OP0("<=")

bool operator>(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, >)
{
	return a.f > b.f;
} CHECKED_IMPL_END_OP0(">")

bool operator>=(EmsHardFloat a, EmsHardFloat b) CHECKED_IMPL_BEGIN_OP2_BOOL(a, b, >=)
{
	return a.f >= b.f;
} CHECKED_IMPL_END_OP0(">=")

EmsHardFloat abs(EmsHardFloat a) CHECKED_IMPL_BEGIN(a, abs)
{
	return EmsHardFloat(std::abs(a.f));
} CHECKED_IMPL_END("abs")

EmsHardFloat floor(EmsHardFloat a) CHECKED_IMPL_BEGIN(a, floor)
{
	return EmsHardFloat(std::floor(a.f));
} CHECKED_IMPL_END("floor")

EmsHardFloat ceil(EmsHardFloat a) CHECKED_IMPL_BEGIN(a, ceil)
{
	return EmsHardFloat(std::ceil(a.f));
} CHECKED_IMPL_END("ceil")

EmsHardFloat trunc(EmsHardFloat a) CHECKED_IMPL_BEGIN(a, trunc)
{
	return EmsHardFloat(std::trunc(a.f));
} CHECKED_IMPL_END("trunc")

EmsHardFloat sqrt(EmsHardFloat a) CHECKED_IMPL_BEGIN(a, sqrt)
{
	return EmsHardFloat(std::sqrt(a.f));
} CHECKED_IMPL_END("sqrt")

EmsHardFloat sin(EmsHardFloat a) CHECKED_IMPL_BEGIN(a, sin)
{
	// handle negative values
	if(a < EmsHardFloat()) return -sin(-a);

	// reduce to range [0, pi]
	EmsHardFloat n;
	if(a > EmsHardFloat::pi)
	{
		n = trunc(a / EmsHardFloat::pi);
		a -= n * EmsHardFloat::pi;
	}
	// reduce to range [0, pi/2]
	if(a > EmsHardFloat::pi_2) a = EmsHardFloat::pi - a;

	static const EmsHardFloat
		k1 = EmsHardFloat::fromUint32Const(0x3f7ff052), //  0.99976073735983227f
		k3 = EmsHardFloat::fromUint32Const(0xbe29c7cc), // -0.16580121984779175f
		k5 = EmsHardFloat::fromUint32Const(0x3bf7d14a); //  0.00756279111686865f

	EmsHardFloat a2 = a * a, a3 = a2 * a, a5 = a3 * a2;

	a = a * k1 + a3 * k3 + a5 * k5;

	return ((int)n % 2 == 0) ? a : -a;
} CHECKED_IMPL_END("sin")

EmsHardFloat cos(EmsHardFloat a) CHECKED_IMPL_BEGIN(a, cos)
{
	return sin(a + EmsHardFloat::pi_2);
} CHECKED_IMPL_END("cos")

EmsHardFloat atan(EmsHardFloat a) CHECKED_IMPL_BEGIN(a, atan)
{
	// handle negative values
	if(a < EmsHardFloat()) return -atan(-a);

	if(a > EmsHardFloat(1.0f)) return EmsHardFloat::pi_2 - atan(EmsHardFloat(1.0f) / a);

	static const EmsHardFloat
		k1 = EmsHardFloat::fromUint32Const(0x3f7ff50e), //  0.9998329769337240f
		k3 = EmsHardFloat::fromUint32Const(0xbea75d21), // -0.3268824051434949f
		k5 = EmsHardFloat::fromUint32Const(0x3e22d89e), //  0.1590294514698240f
		k7 = EmsHardFloat::fromUint32Const(0xbd407012); // -0.0469818803609288f

	EmsHardFloat a2 = a * a, a4 = a2 * a2, a6 = a4 * a2;

	return a * (k1 + a2 * k3 + a4 * k5 + a6 * k7);
} CHECKED_IMPL_END("atan")

EmsHardFloat atan2(EmsHardFloat y, EmsHardFloat x) CHECKED_IMPL_BEGIN2(y, x, atan2)
{
	bool xs = x >= EmsHardFloat();
	if(y == EmsHardFloat()) return xs ? EmsHardFloat() : EmsHardFloat::pi;

	bool ys = y >= EmsHardFloat();
	if(x == EmsHardFloat()) return ys ? EmsHardFloat::pi_2 : (-EmsHardFloat::pi_2);

	EmsHardFloat a = atan(y / x);
	return (xs == ys) ? (xs ? a : (a - EmsHardFloat::pi)) : (xs ? a : (a + EmsHardFloat::pi));
} CHECKED_IMPL_END("atan2")

EmsHardFloat::operator int32_t() const CHECKED_IMPL_BEGIN_OP0(int32_t)
{
	return (int32_t)f;
} CHECKED_IMPL_END_OP0("int32_t")

EmsHardFloat::operator uint32_t() const CHECKED_IMPL_BEGIN_OP0(uint32_t)
{
	return (uint32_t)f;
} CHECKED_IMPL_END_OP0("uint32_t")

EmsHardFloat::operator float() const CHECKED_IMPL_BEGIN_OP0(float)
{
	return f;
} CHECKED_IMPL_END_OP0("float")

EmsHardFloat EmsHardFloat::fromUint32Const(uint32_t a)
{
	EmsHardFloat f;
	f.d = a;
	return f;
}

END_INANITY_MATH

namespace std
{
	Inanity::Math::EmsHardFloat numeric_limits<Inanity::Math::EmsHardFloat>::min()
	{
		return Inanity::Math::EmsHardFloat(numeric_limits<float>::min());
	}

	Inanity::Math::EmsHardFloat numeric_limits<Inanity::Math::EmsHardFloat>::lowest()
	{
		return Inanity::Math::EmsHardFloat(numeric_limits<float>::lowest());
	}

	Inanity::Math::EmsHardFloat numeric_limits<Inanity::Math::EmsHardFloat>::max()
	{
		return Inanity::Math::EmsHardFloat(numeric_limits<float>::max());
	}
}
