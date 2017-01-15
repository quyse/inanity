#include "softfloat.hpp"
#include <cmath>
extern "C" {
#include "softfloat.h"
}

BEGIN_INANITY namespace SoftFloat {

Float::Float()
{
	d.v = 0;
}

Float::Float(float32_t d) : d(d) {}

Float::Float(int32_t i) : d(i32_to_f32(i)) {}

Float::Float(uint32_t i) : d(ui32_to_f32(i)) {}

Float::Float(float f) : d(*reinterpret_cast<float32_t*>(&f)) {}

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
	return Float(f32_rem(a.d, b.d));
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

Float abs(Float a)
{
	return a < Float() ? -a : a;
}

Float floor(Float a)
{
	return Float(f32_roundToInt(a.d, softfloat_round_minMag, false));
}

Float ceil(Float a)
{
	return Float(f32_roundToInt(a.d, softfloat_round_near_maxMag, false));
}

Float sqrt(Float a)
{
	return Float(f32_sqrt(a.d));
}

Float sin(Float a)
{
	// FIX ME
	return Float(std::sin((float)a));
}

Float cos(Float a)
{
	// FIX ME
	return Float(std::cos((float)a));
}

Float atan2(Float y, Float x)
{
	// FIX ME
	return Float(std::atan2((float)y, (float)x));
}

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
	return *reinterpret_cast<const float*>(&d);
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
