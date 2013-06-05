#ifndef ___INANITY_META_TUPLE_HPP___
#define ___INANITY_META_TUPLE_HPP___

#include "meta.hpp"

BEGIN_INANITY_META

/// The structure which stores values of different types.
template <typename First, typename Rest>
struct Tuple
{
	First first;
	Rest rest;
};
// Specialization for no values.
template <>
struct Tuple<void, void>
{
};
typedef Tuple<void, void> VoidTuple;

END_INANITY_META

#endif
