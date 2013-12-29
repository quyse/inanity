#ifndef ___INANITY_SCRIPT_NP_META_PROVIDER_IPP___
#define ___INANITY_SCRIPT_NP_META_PROVIDER_IPP___

#include "MetaProvider.hpp"
#include "../../meta/ClassBase.ipp"
#include "wrappers.hpp"
#include "thunks.ipp"
#include <unordered_map>

BEGIN_INANITY_NP

template <typename ConstructorType>
MetaProvider::FunctionRoutine MetaProvider::Constructor<ConstructorType>::GetThunk() const
{
	return &ConstructorThunk<ConstructorType>::Thunk;
}

template <typename FunctionType, FunctionType function>
MetaProvider::FunctionRoutine MetaProvider::Function<FunctionType, function>::GetThunk() const
{
	return &FunctionThunk<FunctionType, function>::Thunk;
}

template <typename MethodType, MethodType method>
MetaProvider::MethodRoutine MetaProvider::Method<MethodType, method>::GetThunk() const
{
	return &MethodThunk<MethodType, method>::Thunk;
}

class MetaProvider::ClassBase : public Meta::ClassBase<Traits>
{
public:
	typedef std::unordered_map<NPIdentifier, int> MethodsByIdentifier;

protected:
	NPClassWrapper npClassWrapper;
	/// Map of method indices by NP identifiers.
	MethodsByIdentifier methodsByIdentifier;

public:
	ClassBase(const char* name, const char* fullName) :
		Meta::ClassBase<Traits>(name, fullName),
		npClassWrapper(this)
	{}

	void AddMethod(MethodBase* method)
	{
		methodsByIdentifier.insert(
			std::make_pair(
				Platform::NpapiPlugin::browserFuncs.getstringidentifier(
					method->GetName()),
				(int)methods.size()));
		Meta::ClassBase<Traits>::AddMethod(method);
	}

	NPClassWrapper* GetClassWrapper()
	{
		return &npClassWrapper;
	}

	const MethodsByIdentifier& GetMethodsByIdentifier() const
	{
		return methodsByIdentifier;
	}
};

template <typename ClassType>
class MetaProvider::Class : public MetaProvider::ClassBase
{
public:
	Class(const char* name, const char* fullName)
	: ClassBase(name, fullName) {}
};

END_INANITY_NP

#endif
