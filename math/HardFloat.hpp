#ifndef ___INANITY_MATH_HARD_FLOAT_HPP___
#define ___INANITY_MATH_HARD_FLOAT_HPP___

#include "math.hpp"
#include <limits>

BEGIN_INANITY_MATH

/// Class of IEEE 754 deterministic float, based on SSE2.
/** Performs all operations in 32-bit precision.
Should be unaffected by compiler optimizations, etc. */
class HardFloat
{
public:
	HardFloat();
	HardFloat(const HardFloat&) = default;

	explicit HardFloat(int32_t);
	explicit HardFloat(uint32_t);
	explicit HardFloat(float);

	HardFloat operator-() const;

	friend HardFloat operator+(HardFloat, HardFloat);
	friend HardFloat operator-(HardFloat, HardFloat);
	friend HardFloat operator*(HardFloat, HardFloat);
	friend HardFloat operator/(HardFloat, HardFloat);
	friend HardFloat fmod(HardFloat, HardFloat);

	HardFloat& operator+=(HardFloat);
	HardFloat& operator-=(HardFloat);
	HardFloat& operator*=(HardFloat);
	HardFloat& operator/=(HardFloat);

	friend bool operator==(HardFloat, HardFloat);
	friend bool operator!=(HardFloat, HardFloat);
	friend bool operator<(HardFloat, HardFloat);
	friend bool operator<=(HardFloat, HardFloat);
	friend bool operator>(HardFloat, HardFloat);
	friend bool operator>=(HardFloat, HardFloat);

	friend HardFloat abs(HardFloat);
	friend HardFloat floor(HardFloat);
	friend HardFloat ceil(HardFloat);
	friend HardFloat trunc(HardFloat);
	friend HardFloat sqrt(HardFloat);
	friend HardFloat sin(HardFloat);
	friend HardFloat cos(HardFloat);
	friend HardFloat atan(HardFloat);
	friend HardFloat atan2(HardFloat, HardFloat);

	explicit operator int32_t() const;
	explicit operator uint32_t() const;
	explicit operator float() const;

	uint32_t getInternalUint32() const;
	static HardFloat fromUint32Const(uint32_t);

private:
	static const HardFloat pi, pi2, pi_2;

	float f;
};

END_INANITY_MATH

namespace std
{
	template <>
	class numeric_limits<Inanity::Math::HardFloat> : public numeric_limits<float>
	{
	public:
		static constexpr float_round_style round_style = round_to_nearest;
		static constexpr bool is_iec559 = true;
		static Inanity::Math::HardFloat min();
		static Inanity::Math::HardFloat lowest();
		static Inanity::Math::HardFloat max();
	};
}

#endif
