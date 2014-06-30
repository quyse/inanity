#ifndef ___INANITY_META_CALLABLE_IPP___
#define ___INANITY_META_CALLABLE_IPP___

#include "Callable.hpp"
#include "Tuple.hpp"

BEGIN_INANITY_META

/// Structure for keeping a sequence of numbers.
template <int...>
struct TupleSeq
{
};
/// Structure for generating a sequence of numbers.
template <int N, int... S>
struct TupleSeqGen
{
	typedef typename TupleSeqGen<N-1, N-1, S...>::Type Type;
};
template <int... S>
struct TupleSeqGen<0, S...>
{
	typedef TupleSeq<S...> Type;
};

/// Structure to help call functions and methods.
template <typename S, typename CalleeType>
struct CallableHelper;
/// Structure to help call constructors.
template <typename S, typename C, typename... A>
struct CallableConstructorHelper;

//******* Callable for functions

template <int... N, typename R, typename... A>
struct CallableHelper<TupleSeq<N...>, R (*)(A...)>
{
	static inline R Call(R (*callee)(A...), const Tuple<A...>& args)
	{
		return callee(TupleArg<N, A...>::Get(args)...);
	}
};

template <typename R, typename... A>
struct Callable<R (*)(A...)>
{
	enum { isMethod = 0 };
	typedef R (*CalleeType)(A...);
	typedef Tuple<A...> Args;
	typedef R ReturnType;
	static inline R Call(CalleeType callee, const Args& args)
	{
		return CallableHelper<typename TupleSeqGen<Args::length>::Type, CalleeType>::Call(callee, args);
	}
};

//******* Callable for non-const methods

template <int... N, typename R, typename C, typename... A>
struct CallableHelper<TupleSeq<N...>, R (C::*)(A...)>
{
	static inline R Call(C* c, R (C::*callee)(A...), const Tuple<A...>& args)
	{
		return (c->*callee)(TupleArg<N, A...>::Get(args)...);
	}
};

template <typename R, typename C, typename... A>
struct Callable<R (C::*)(A...)>
{
	enum { isMethod = 0 };
	typedef R (C::*CalleeType)(A...);
	typedef Tuple<ptr<C>, A...> Args;
	typedef R ReturnType;
	static inline R Call(CalleeType callee, const Args& args)
	{
		return CallableHelper<typename TupleSeqGen<Tuple<A...>::length>::Type, CalleeType>::Call((C*)args.first, callee, args.rest);
	}
};

//******* Callable for const methods

template <int... N, typename R, typename C, typename... A>
struct CallableHelper<TupleSeq<N...>, R (C::*)(A...) const>
{
	static inline R Call(C* c, R (C::*callee)(A...) const, const Tuple<A...>& args)
	{
		return (c->*callee)(TupleArg<N, A...>::Get(args)...);
	}
};

template <typename R, typename C, typename... A>
struct Callable<R (C::*)(A...) const>
{
	enum { isMethod = 0 };
	typedef R (C::*CalleeType)(A...) const;
	typedef Tuple<ptr<C>, A...> Args;
	typedef R ReturnType;
	static inline R Call(CalleeType callee, const Args& args)
	{
		return CallableHelper<typename TupleSeqGen<Tuple<A...>::length>::Type, CalleeType>::Call((C*)args.first, callee, args.rest);
	}
};

//******* Callable for constructors

template <int... N, typename C, typename... A>
struct CallableConstructorHelper<TupleSeq<N...>, C, A...>
{
	static inline ptr<C> Call(const Tuple<A...>& args)
	{
		return NEW(C(TupleArg<N, A...>::Get(args)...));
	}
};

template <typename C, typename... A>
struct CallableConstructor
{
	enum { isMethod = 0 };
	typedef Tuple<A...> Args;
	typedef ptr<C> ReturnType;
	typedef C ClassType;
	static inline ptr<C> Call(const Args& args)
	{
		return CallableConstructorHelper<typename TupleSeqGen<Tuple<A...>::length>::Type, C, A...>::Call(args);
	}
};

END_INANITY_META

#endif
