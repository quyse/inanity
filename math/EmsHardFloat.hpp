#ifndef ___INANITY_MATH_EMS_HARD_FLOAT_HPP___
#define ___INANITY_MATH_EMS_HARD_FLOAT_HPP___

#include "math.hpp"
#include <limits>
#include <emmintrin.h>

BEGIN_INANITY_MATH

/// Class of IEEE 754 deterministic float for emscripten.
/** Mostly just a wrapper over normal float except for transcendent functions.
For correct work the .cpp has to be compiled with -s PRECISE_F32=1
and without -ffast-math. */
class EmsHardFloat
{
public:
	EmsHardFloat();
	EmsHardFloat(const EmsHardFloat&) = default;

	explicit EmsHardFloat(int32_t);
	explicit EmsHardFloat(uint32_t);
	explicit EmsHardFloat(float);

	EmsHardFloat operator-() const;

	friend EmsHardFloat operator+(EmsHardFloat, EmsHardFloat);
	friend EmsHardFloat operator-(EmsHardFloat, EmsHardFloat);
	friend EmsHardFloat operator*(EmsHardFloat, EmsHardFloat);
	friend EmsHardFloat operator/(EmsHardFloat, EmsHardFloat);
	friend EmsHardFloat fmod(EmsHardFloat, EmsHardFloat);

	EmsHardFloat& operator+=(EmsHardFloat);
	EmsHardFloat& operator-=(EmsHardFloat);
	EmsHardFloat& operator*=(EmsHardFloat);
	EmsHardFloat& operator/=(EmsHardFloat);

	friend bool operator==(EmsHardFloat, EmsHardFloat);
	friend bool operator!=(EmsHardFloat, EmsHardFloat);
	friend bool operator<(EmsHardFloat, EmsHardFloat);
	friend bool operator<=(EmsHardFloat, EmsHardFloat);
	friend bool operator>(EmsHardFloat, EmsHardFloat);
	friend bool operator>=(EmsHardFloat, EmsHardFloat);

	friend EmsHardFloat abs(EmsHardFloat);
	friend EmsHardFloat floor(EmsHardFloat);
	friend EmsHardFloat ceil(EmsHardFloat);
	friend EmsHardFloat trunc(EmsHardFloat);
	friend EmsHardFloat sqrt(EmsHardFloat);
	friend EmsHardFloat sin(EmsHardFloat);
	friend EmsHardFloat cos(EmsHardFloat);
	friend EmsHardFloat atan(EmsHardFloat);
	friend EmsHardFloat atan2(EmsHardFloat, EmsHardFloat);

	explicit operator int32_t() const;
	explicit operator uint32_t() const;
	explicit operator float() const;

	static EmsHardFloat fromUint32Const(uint32_t);

private:
	static const EmsHardFloat pi, pi2, pi_2;

	union
	{
		float f;
		uint32_t d;
	};
};

END_INANITY_MATH

namespace std
{
	template <>
	class numeric_limits<Inanity::Math::EmsHardFloat>
	{
	public:
		static constexpr float_round_style round_style = round_to_nearest;
		static constexpr bool is_iec559 = true;
		static Inanity::Math::EmsHardFloat min();
		static Inanity::Math::EmsHardFloat lowest();
		static Inanity::Math::EmsHardFloat max();
	};
}

#endif
