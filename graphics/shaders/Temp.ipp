#ifndef ___INANITY_GRAPHICS_SHADERS_TEMP_IPP___
#define ___INANITY_GRAPHICS_SHADERS_TEMP_IPP___

#include "Temp.hpp"
#include "TempNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Temp<ValueType>::Temp()
: LValue<ValueType>(NEW(TempNode(DataTypeOf<ValueType>())))
{}

template <typename ValueType>
Value<ValueType> Temp<ValueType>::operator=(const Temp<ValueType>& a)
{
	return LValue<ValueType>::operator=(static_cast<const Value<ValueType>&>(a));
}

template <typename ValueType>
Value<ValueType> Temp<ValueType>::operator=(const Value<ValueType>& a)
{
	return LValue<ValueType>::operator=(a);
}

END_INANITY_SHADERS

#endif
