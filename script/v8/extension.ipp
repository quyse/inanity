#include "extension.hpp"
#include "thunks.ipp"

BEGIN_INANITY_V8

template <typename ConstructorType>
v8::FunctionCallback Extension<Meta::Constructor<ConstructorType> >::GetThunk() const
{
	return &ConstructorThunk<ConstructorType>::Thunk;
}

template <typename FunctionType, FunctionType function>
v8::FunctionCallback Extension<Meta::Function<FunctionType, function> >::GetThunk() const
{
	return &CalleeThunk<FunctionType, function>::Thunk;
}

template <typename MethodType, MethodType method>
v8::FunctionCallback Extension<Meta::Method<MethodType, method> >::GetThunk() const
{
	return &CalleeThunk<MethodType, method>::Thunk;
}

END_INANITY_V8
