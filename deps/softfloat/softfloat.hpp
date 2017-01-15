#ifndef ___INANITY_SOFTFLOAT_HPP___
#define ___INANITY_SOFTFLOAT_HPP___

#include "../../Object.hpp"
#include "softfloat/source/include/softfloat_types.h"
#include <limits>

BEGIN_INANITY namespace SoftFloat {

class Float
{
public:
	Float();
	Float(const Float&) = default;

	explicit Float(int32_t);
	explicit Float(uint32_t);
	explicit Float(float);

	Float operator-() const;

	friend Float operator+(Float, Float);
	friend Float operator-(Float, Float);
	friend Float operator*(Float, Float);
	friend Float operator/(Float, Float);
	friend Float fmod(Float, Float);

	Float& operator+=(Float);
	Float& operator-=(Float);
	Float& operator*=(Float);
	Float& operator/=(Float);

	friend bool operator==(Float, Float);
	friend bool operator!=(Float, Float);
	friend bool operator<(Float, Float);
	friend bool operator<=(Float, Float);
	friend bool operator>(Float, Float);
	friend bool operator>=(Float, Float);

	friend Float abs(Float);
	friend Float floor(Float);
	friend Float ceil(Float);
	friend Float sqrt(Float);
	friend Float sin(Float);
	friend Float cos(Float);
	friend Float atan2(Float, Float);

	explicit operator int32_t() const;
	explicit operator uint32_t() const;
	explicit operator int16_t() const;
	explicit operator uint16_t() const;
	explicit operator int8_t() const;
	explicit operator uint8_t() const;
	explicit operator float() const;

private:
	explicit Float(float32_t);

	float32_t d;
};

} END_INANITY

namespace std
{
	template <>
	class numeric_limits<Inanity::SoftFloat::Float> : public numeric_limits<float>
	{
	public:
		static constexpr float_round_style round_style = round_to_nearest;
		static constexpr bool is_iec559 = true;
		static Inanity::SoftFloat::Float min();
		static Inanity::SoftFloat::Float lowest();
		static Inanity::SoftFloat::Float max();
	};
}

#endif
