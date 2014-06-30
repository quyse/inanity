#ifndef ___INANITY_SCRIPT_V8_META_PROVIDER_IPP___
#define ___INANITY_SCRIPT_V8_META_PROVIDER_IPP___

#include "MetaProvider.hpp"
#include "../../meta/ClassBase.hpp"
#include "thunks.hpp"

BEGIN_INANITY_V8

//*** class MetaProvider::Constructor

template <typename ClassType, typename... Args>
v8::FunctionCallback MetaProvider::Constructor<ClassType, Args...>::GetThunk() const
{
	return &ConstructorThunk<ClassType, Args...>::Thunk;
}

//*** class MetaProvider::Function

template <typename FunctionType, FunctionType function>
MetaProvider::Function<FunctionType, function>::Function(const char* name)
: FunctionBase(name) {}

template <typename FunctionType, FunctionType function>
v8::FunctionCallback MetaProvider::Function<FunctionType, function>::GetThunk() const
{
	return &CalleeThunk<FunctionType, function>::Thunk;
}

//*** class MetaProvider::Method

template <typename MethodType, MethodType method>
MetaProvider::Method<MethodType, method>::Method(const char* name)
: MethodBase(name) {}

template <typename MethodType, MethodType method>
v8::FunctionCallback MetaProvider::Method<MethodType, method>::GetThunk() const
{
	return &CalleeThunk<MethodType, method>::Thunk;
}

//*** class MetaProvider::Class

template <typename ClassType>
MetaProvider::Class<ClassType>::Class(const char* name, const char* fullName)
: ClassBase(name, fullName) {}

template <typename ClassType>
v8::FunctionCallback MetaProvider::Class<ClassType>::GetDummyConstructorThunk() const
{
	return &DummyConstructorThunk<ClassType>;
}

END_INANITY_V8

#endif
