#ifndef ___INANITY_SCRIPT_V8_META_PROVIDER_IPP___
#define ___INANITY_SCRIPT_V8_META_PROVIDER_IPP___

#include "MetaProvider.hpp"
#include "../../meta/ClassBase.ipp"
#include "thunks.ipp"

BEGIN_INANITY_V8

template <typename ConstructorType>
v8::FunctionCallback MetaProvider::Constructor<ConstructorType>::GetThunk() const
{
	return &ConstructorThunk<ConstructorType>::Thunk;
}

template <typename FunctionType, FunctionType function>
v8::FunctionCallback MetaProvider::Function<FunctionType, function>::GetThunk() const
{
	return &CalleeThunk<FunctionType, function>::Thunk;
}

template <typename MethodType, MethodType method>
v8::FunctionCallback MetaProvider::Method<MethodType, method>::GetThunk() const
{
	return &CalleeThunk<MethodType, method>::Thunk;
}

END_INANITY_V8

#endif
