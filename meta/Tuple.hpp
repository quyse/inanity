#ifndef ___INANITY_META_TUPLE_HPP___
#define ___INANITY_META_TUPLE_HPP___

#include "meta.hpp"

BEGIN_INANITY_META

/// This structure removes reference from type.
template <typename SourceType>
struct Storagable
{
	typedef SourceType Type;
};
template <typename SourceType>
struct Storagable<const SourceType&>
{
	typedef SourceType Type;
};

/// The structure which stores values of different types.
template <typename First, typename Rest>
struct Tuple
{
	typename Storagable<First>::Type first;
	Rest rest;

	template <typename Initializer>
	Tuple(Initializer& initializer) :
		first(initializer.template Get<First>()),
		rest(initializer)
	{}
};
// Specialization for no values.
template <>
struct Tuple<void, void>
{
	template <typename Initializer>
	Tuple(Initializer&) {}
};
typedef Tuple<void, void> VoidTuple;

END_INANITY_META

#endif
