#ifndef ___INANITY_SCRIPT_NP_META_PROVIDER_IPP___
#define ___INANITY_SCRIPT_NP_META_PROVIDER_IPP___

#include "MetaProvider.hpp"
#include "ClassBase.hpp"
#include "thunks.hpp"

BEGIN_INANITY_NP

//*** class MetaProvider::Constructor

template <typename ConstructorType>
MetaProvider::FunctionRoutine MetaProvider::Constructor<ConstructorType>::GetThunk() const
{
	return &ConstructorThunk<ConstructorType>::Thunk;
}

//*** class MetaProvider::Function

template <typename FunctionType, FunctionType function>
MetaProvider::Function<FunctionType, function>::Function(const char* name)
: FunctionBase(name) {}

template <typename FunctionType, FunctionType function>
MetaProvider::FunctionRoutine MetaProvider::Function<FunctionType, function>::GetThunk() const
{
	return &FunctionThunk<FunctionType, function>::Thunk;
}

//*** class MetaProvider::Method

template <typename MethodType, MethodType method>
MetaProvider::Method<MethodType, method>::Method(const char* name)
: MethodBase(name) {}

template <typename MethodType, MethodType method>
MetaProvider::MethodRoutine MetaProvider::Method<MethodType, method>::GetThunk() const
{
	return &MethodThunk<MethodType, method>::Thunk;
}

//*** class MetaProvider::Class

template <typename ClassType>
class MetaProvider::Class : public MetaProvider::ClassBase
{
public:
	Class(const char* name, const char* fullName)
	: ClassBase(name, fullName) {}
};

END_INANITY_NP

#endif
