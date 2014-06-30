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
/** Main template works only for empty tuple. */
template <typename... T>
struct Tuple
{
	enum { length = 0 };

	template <typename Initializer>
	Tuple(Initializer& initializer) {}
};
// Specialization for non-empty tuple.
template <typename First, typename... Rest>
struct Tuple<First, Rest...>
{
	enum { length = 1 + Tuple<Rest...>::length };

	typename Storagable<First>::Type first;
	Tuple<Rest...> rest;

	template <typename Initializer>
	Tuple(Initializer& initializer) :
		first(initializer.template Get<First>()),
		rest(initializer)
	{}
};

/// Structure which can get an argument from tuple.
template <int, typename...>
struct TupleArg
{
};
template <int N, typename First, typename... Rest>
struct TupleArg<N, First, Rest...>
{
	typedef typename TupleArg<N-1, Rest...>::Type Type;
	static inline auto Get(const Tuple<First, Rest...>& tuple)
		-> decltype(TupleArg<N-1, Rest...>::Get(tuple.rest))
	{
		return TupleArg<N-1, Rest...>::Get(tuple.rest);
	}
};
template <typename First, typename... Rest>
struct TupleArg<0, First, Rest...>
{
	typedef First Type;
	static inline auto Get(const Tuple<First, Rest...>& tuple) -> First
	{
		return tuple.first;
	}
};

END_INANITY_META

#endif
