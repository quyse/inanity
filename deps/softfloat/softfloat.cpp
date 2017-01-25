#include "softfloat.hpp"
#include <cmath>
#include <cassert>
extern "C" {
#include "softfloat.h"
}

#ifdef _DEBUG
#define CHECKED_IMPL_BEGIN(x, f) \
	{ \
		float theirs = f((float)x); \
		Float ours = ([&x]()
#define CHECKED_IMPL_BEGIN2(a, b, f) \
	{ \
		float theirs = f((float)a, (float)b); \
		Float ours = ([&a, &b]()
#define CHECKED_IMPL_END() \
	)(); \
		assert(std::abs(theirs - (float)ours) < 0.001f); \
		return ours; \
	}
#else
#define CHECKED_IMPL_BEGIN(x, f)
#define CHECKED_IMPL_BEGIN2(a, b, f)
#define CHECKED_IMPL_END()
#endif

BEGIN_INANITY namespace SoftFloat {

const Float Float::pi = Float::fromUint32Const(0x40490fdb);
const Float Float::pi2 = Float::fromUint32Const(0x40c90fdb);
const Float Float::pi_2 = Float::fromUint32Const(0x3fc90fdb);

Float::Float()
{
	d.v = 0;
}

Float::Float(float32_t d) : d(d) {}

Float::Float(int32_t i) : d(i32_to_f32(i)) {}

Float::Float(uint32_t i) : d(ui32_to_f32(i)) {}

Float::Float(float f) : f(f) {}

Float Float::fromUint32Const(uint32_t v)
{
	float32_t d;
	d.v = v;
	return Float(d);
}

Float Float::operator-() const
{
	return Float() - *this;
}

Float operator+(Float a, Float b)
{
	return Float(f32_add(a.d, b.d));
}

Float operator-(Float a, Float b)
{
	return Float(f32_sub(a.d, b.d));
}

Float operator*(Float a, Float b)
{
	return Float(f32_mul(a.d, b.d));
}

Float operator/(Float a, Float b)
{
	return Float(f32_div(a.d, b.d));
}

Float fmod(Float a, Float b)
{
	return a - Float(f32_roundToInt(f32_div(a.d, b.d), softfloat_round_minMag, false)) * b;
}

Float& Float::operator+=(Float b)
{
	d = f32_add(d, b.d);
	return *this;
}

Float& Float::operator-=(Float b)
{
	d = f32_sub(d, b.d);
	return *this;
}

Float& Float::operator*=(Float b)
{
	d = f32_mul(d, b.d);
	return *this;
}

Float& Float::operator/=(Float b)
{
	d = f32_div(d, b.d);
	return *this;
}

bool operator==(Float a, Float b)
{
	return f32_eq(a.d, b.d);
}

bool operator!=(Float a, Float b)
{
	return !(a == b);
}

bool operator<(Float a, Float b)
{
	return f32_lt(a.d, b.d);
}

bool operator<=(Float a, Float b)
{
	return f32_le(a.d, b.d);
}

bool operator>(Float a, Float b)
{
	return b < a;
}

bool operator>=(Float a, Float b)
{
	return b <= a;
}

Float abs(Float a) CHECKED_IMPL_BEGIN(a, std::abs)
{
	return a < Float() ? -a : a;
} CHECKED_IMPL_END()

Float floor(Float a) CHECKED_IMPL_BEGIN(a, std::floor)
{
	return Float(f32_roundToInt(a.d, softfloat_round_min, false));
} CHECKED_IMPL_END()

Float ceil(Float a) CHECKED_IMPL_BEGIN(a, std::ceil)
{
	return Float(f32_roundToInt(a.d, softfloat_round_max, false));
} CHECKED_IMPL_END()

Float sqrt(Float a) CHECKED_IMPL_BEGIN(a, std::sqrt)
{
	return Float(f32_sqrt(a.d));
} CHECKED_IMPL_END()

Float sin(Float a) CHECKED_IMPL_BEGIN(a, std::sin)
{
	// handle negative values
	if(a < Float()) return -sin(-a);

	// reduce to range [0, pi]
	Float n;
	if(a > Float::pi)
	{
		n = Float(f32_roundToInt(f32_div(a.d, Float::pi.d), softfloat_round_minMag, false));
		a -= n * Float::pi;
	}
	// reduce to range [0, pi/2]
	if(a > Float::pi_2) a = Float::pi - a;

	static const Float
		k1 = Float::fromUint32Const(0x3f7ff052), //  0.99976073735983227f
		k3 = Float::fromUint32Const(0xbe29c7cc), // -0.16580121984779175f
		k5 = Float::fromUint32Const(0x3bf7d14a); //  0.00756279111686865f

	Float a2 = a * a, a3 = a2 * a, a5 = a3 * a2;

	a = a * k1 + a3 * k3 + a5 * k5;

	return ((uint32_t)n % 2 == 0) ? a : -a;
} CHECKED_IMPL_END()

Float cos(Float a) CHECKED_IMPL_BEGIN(a, std::cos)
{
	return sin(a + Float::pi_2);
} CHECKED_IMPL_END()

Float atan(Float a) CHECKED_IMPL_BEGIN(a, std::atan)
{
	// handle negative values
	if(a < Float()) return -atan(-a);

	if(a > Float((uint32_t)1)) return Float::pi_2 - atan(Float((uint32_t)1) / a);

	static const Float
		k1 = Float::fromUint32Const(0x3f7ff50e), //  0.9998329769337240f
		k3 = Float::fromUint32Const(0xbea75d21), // -0.3268824051434949f
		k5 = Float::fromUint32Const(0x3e22d89e), //  0.1590294514698240f
		k7 = Float::fromUint32Const(0xbd407012); // -0.0469818803609288f

	Float a2 = a * a, a4 = a2 * a2, a6 = a4 * a2;

	return a * (k1 + a2 * k3 + a4 * k5 + a6 * k7);
} CHECKED_IMPL_END()

Float atan2(Float y, Float x) CHECKED_IMPL_BEGIN2(y, x, std::atan2)
{
	bool xs = x >= Float();
	if(y == Float()) return xs ? Float() : Float::pi;

	bool ys = y >= Float();
	if(x == Float()) return ys ? Float::pi_2 : (-Float::pi_2);

	Float a = atan(y / x);
	return (xs == ys) ? (xs ? a : (a - Float::pi)) : (xs ? a : (a + Float::pi));
} CHECKED_IMPL_END()

Float::operator int32_t() const
{
	return f32_to_i32_r_minMag(d, false);
}

Float::operator uint32_t() const
{
	return f32_to_ui32_r_minMag(d, false);
}

Float::operator int16_t() const
{
	return (int16_t)f32_to_i32_r_minMag(d, false);
}

Float::operator uint16_t() const
{
	return (uint16_t)f32_to_ui32_r_minMag(d, false);
}

Float::operator int8_t() const
{
	return (int8_t)f32_to_i32_r_minMag(d, false);
}

Float::operator uint8_t() const
{
	return (uint8_t)f32_to_ui32_r_minMag(d, false);
}

Float::operator float() const
{
	return f;
}

} END_INANITY

namespace std
{
	Inanity::SoftFloat::Float numeric_limits<Inanity::SoftFloat::Float>::min()
	{
		return Inanity::SoftFloat::Float(numeric_limits<float>::min());
	}

	Inanity::SoftFloat::Float numeric_limits<Inanity::SoftFloat::Float>::lowest()
	{
		return Inanity::SoftFloat::Float(numeric_limits<float>::lowest());
	}

	Inanity::SoftFloat::Float numeric_limits<Inanity::SoftFloat::Float>::max()
	{
		return Inanity::SoftFloat::Float(numeric_limits<float>::max());
	}
}
