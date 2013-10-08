#ifndef ___INANITY_INANITY_MATH_SCRIPT_IPP___
#define ___INANITY_INANITY_MATH_SCRIPT_IPP___

#include "math/basic.hpp"

BEGIN_INANITY_SCRIPT

//*** xvec<T, n> conversion

template <typename T, int n>
struct ConverterFromScript<Math::xvec<T, n> >
{
	static Math::xvec<T, n> Convert(ptr<Any> value)
	{
		Math::xvec<T, n> r;
		for(int i = 0; i < n; ++i)
			r(i) = (T)*value->Get(i);
		return r;
	}
};

template Math::xvec<float, 2> ConvertFromScript<Math::xvec<float, 2> >(ptr<Any> value);
template Math::xvec<float, 3> ConvertFromScript<Math::xvec<float, 3> >(ptr<Any> value);
template Math::xvec<float, 4> ConvertFromScript<Math::xvec<float, 4> >(ptr<Any> value);

template <typename T, int n>
struct ConverterToScript<Math::xvec<T, n> >
{
	static ptr<Any> Convert(ptr<State> state, const Math::xvec<T, n>& value)
	{
		ptr<Any> array = state->NewArray(n);
		for(int i = 0; i < n; ++i)
			array->Set(i, state->NewNumber(value(i)));
		return array;
	}
};

template ptr<Any> ConvertToScript<Math::xvec<float, 2> >(ptr<State> state, const Math::xvec<float, 2>& value);
template ptr<Any> ConvertToScript<Math::xvec<float, 3> >(ptr<State> state, const Math::xvec<float, 3>& value);
template ptr<Any> ConvertToScript<Math::xvec<float, 4> >(ptr<State> state, const Math::xvec<float, 4>& value);

//*** xmat<T, n, m> conversion

template <typename T, int n, int m>
struct ConverterFromScript<Math::xmat<T, n, m> >
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

template Math::xmat<float, 4, 4> ConvertFromScript<Math::xmat<float, 4, 4> >(ptr<Any> value);

template <typename T, int n, int m>
struct ConverterToScript<Math::xmat<T, n, m> >
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

template ptr<Any> ConvertToScript<Math::xmat<float, 4, 4> >(ptr<State> state, const Math::xmat<float, 4, 4>& value);

END_INANITY_SCRIPT

#endif
