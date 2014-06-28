#ifndef ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_IPP___
#define ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_IPP___

#include "Interpolant.hpp"
#include "TransformedNode.hpp"
#include "InterpolateNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Interpolant<ValueType>::Interpolant(int semantic)
: Value<ValueType>(NEW(TransformedNode(DataTypeOf<ValueType>(), semantic))), semantic(semantic)
{}

template <typename ValueType>
Expression Interpolant<ValueType>::Set(const Value<ValueType>& a)
{
	return Expression(NEW(InterpolateNode(DataTypeOf<ValueType>(), semantic, a.GetNode())));
}

END_INANITY_SHADERS

#endif
