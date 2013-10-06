#ifndef ___INANITY_INANITY_MATH_META_IPP___
#define ___INANITY_INANITY_MATH_META_IPP___

#include "math/basic.hpp"

BEGIN_INANITY_META

//*** xvec<T, n> conversion

template <typename T, int n>
struct ConverterFromScript<META_PROVIDER, Math::xvec<T, n> >
{
	static Math::xvec<T, n> Convert(const META_PROVIDER::Any& value)
	{
		Math::xvec<T, n> r;
		for(int i = 0; i < n; ++i)
			r(i) = (T)value[i];
		return r;
	}
};

template Math::xvec<float, 2> ConvertFromScript<META_PROVIDER, Math::xvec<float, 2> >(const META_PROVIDER::Any& value);
template Math::xvec<float, 3> ConvertFromScript<META_PROVIDER, Math::xvec<float, 3> >(const META_PROVIDER::Any& value);
template Math::xvec<float, 4> ConvertFromScript<META_PROVIDER, Math::xvec<float, 4> >(const META_PROVIDER::Any& value);

template <typename T, int n>
struct ConverterToScript<META_PROVIDER, Math::xvec<T, n> >
{
	static void Convert(const Math::xvec<T, n>& value, META_PROVIDER::ReturnAny& returnValue)
	{
		returnValue.SetFloatArray(value.t, n);
	}
};

template void ConvertToScript<META_PROVIDER, Math::xvec<float, 2> >(const Math::xvec<float, 2>& value, META_PROVIDER::ReturnAny& returnValue);
template void ConvertToScript<META_PROVIDER, Math::xvec<float, 3> >(const Math::xvec<float, 3>& value, META_PROVIDER::ReturnAny& returnValue);
template void ConvertToScript<META_PROVIDER, Math::xvec<float, 4> >(const Math::xvec<float, 4>& value, META_PROVIDER::ReturnAny& returnValue);

//*** xmat<T, n, m> conversion

template <typename T, int n, int m>
struct ConverterFromScript<META_PROVIDER, Math::xmat<T, n, m> >
{
	static Math::xmat<T, n, m> Convert(const typename META_PROVIDER::Any& value)
	{
		Math::xmat<T, n, m> r;
		int k = 0;
		for(int j = 0; j < m; ++j)
			for(int i = 0; i < n; ++i)
				r(i, j) = (T)value[k++];
		return r;
	}
};

template Math::xmat<float, 4, 4> ConvertFromScript<META_PROVIDER, Math::xmat<float, 4, 4> >(const META_PROVIDER::Any& value);

template <typename T, int n, int m>
struct ConverterToScript<META_PROVIDER, Math::xmat<T, n, m> >
{
	static void Convert(const Math::xmat<T, n, m>& value, typename META_PROVIDER::ReturnAny& returnValue)
	{
		returnValue.SetFloatArray((const float*)value.t, n * m);
	}
};

template void ConvertToScript<META_PROVIDER, Math::xmat<float, 4, 4> >(const Math::xmat<float, 4, 4>& value, META_PROVIDER::ReturnAny& returnValue);

END_INANITY_META

#endif
