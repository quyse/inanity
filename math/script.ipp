#ifndef ___INANITY_MATH_SCRIPT_IPP___
#define ___INANITY_MATH_SCRIPT_IPP___

#include "basic.hpp"
#include "../script/mono/MetaProvider.hpp"

BEGIN_INANITY_SCRIPT

//*** xvec<T, n> conversion

template <typename MetaProvider, typename T, int n>
struct ConverterFromScript<MetaProvider, Math::xvec<T, n> >
{
	static Math::xvec<T, n> Convert(ptr<Any> value)
	{
		Math::xvec<T, n> r;
		for(int i = 0; i < n; ++i)
			r(i) = (T)*value->Get(i);
		return r;
	}
};

template <typename MetaProvider, typename T, int n>
struct ConverterToScript<MetaProvider, Math::xvec<T, n> >
{
	static ptr<Any> Convert(ptr<State> state, const Math::xvec<T, n>& value)
	{
		ptr<Any> array = state->NewArray(n);
		for(int i = 0; i < n; ++i)
			array->Set(i, state->NewNumber(value(i)));
		return array;
	}
};

//*** xmat<T, n, m> conversion

template <typename MetaProvider, typename T, int n, int m>
struct ConverterFromScript<MetaProvider, Math::xmat<T, n, m> >
{
	static Math::xmat<T, n, m> Convert(ptr<Any> value)
	{
		Math::xmat<T, n, m> r;
		int k = 0;
		for(int j = 0; j < m; ++j)
			for(int i = 0; i < n; ++i)
				r(i, j) = (T)*value->Get(k++);
		return r;
	}
};

template <typename MetaProvider, typename T, int n, int m>
struct ConverterToScript<MetaProvider, Math::xmat<T, n, m> >
{
	static ptr<Any> Convert(ptr<State> state, const Math::xmat<T, n, m>& value)
	{
		ptr<Any> array = state->NewArray(n * m);
		int k = 0;
		for(int j = 0; j < m; ++j)
			for(int i = 0; i < n; ++i)
				array->Set(k++, state->NewNumber(value(i, j)));
		return array;
	}
};

END_INANITY_SCRIPT

#endif
