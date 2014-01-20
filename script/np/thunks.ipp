#ifndef ___INANITY_SCRIPT_NP_THUNKS_IPP___
#define ___INANITY_SCRIPT_NP_THUNKS_IPP___

#include "thunks.hpp"
#include "values.ipp"
#include "../../meta/Tuple.hpp"
#include "../../meta/Callable.ipp"
#include "../../platform/NpapiPlugin.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_NP

struct ArgGettingState
{
	const NPVariant* args;
	int count;
	int index;
	NPObject* thisObject;

	ArgGettingState(const NPVariant* args, int count, bool getThis, NPObject* thisObject = nullptr)
	: args(args), count(count), index(getThis ? -1 : 0), thisObject(thisObject) {}

	template <typename ArgType>
	inline typename Value<ArgType>::ValueType Get()
	{
		if(index >= 0)
		{
			if(index >= count)
				THROW("Not enough arguments");
			return Value<ArgType>::From(args[index++]);
		}
		else
		{
			++index;

			NPVariant variant;
			OBJECT_TO_NPVARIANT(thisObject, variant);
			return Value<ArgType>::From(variant);
		}
	}
};

template <typename CallerType, typename ReturnType, typename Args>
struct CallAndReturn
{
	static inline void Do(const Args& args, NPVariant* result)
	{
		*result = Value<ReturnType>::To(CallerType::Call(args));
	}
};
template <typename CallerType, typename Args>
struct CallAndReturn<CallerType, void, Args>
{
	static inline void Do(const Args& args, NPVariant* result)
	{
		CallerType::Call(args);
		VOID_TO_NPVARIANT(*result);
	}
};

template <typename CalleeType, CalleeType callee>
struct FunctionThunk
{
	typedef typename Meta::Callable<CalleeType>::Args Args;
	typedef typename Meta::Callable<CalleeType>::ReturnType ReturnType;

	struct Helper
	{
		static inline ReturnType Call(const Args& args)
		{
			return Meta::Callable<CalleeType>::Call(callee, args);
		}
	};

	static inline bool Thunk(const NPVariant* args, int argsCount, NPVariant* result)
	{
		try
		{
			ArgGettingState state(args, argsCount, false);
			CallAndReturn<Helper, ReturnType, Args>::Do(Args(state), result);
			return true;
		}
		catch(Exception* exception)
		{
			std::ostringstream stream;
			MakePointer(exception)->PrintStack(stream);
			Platform::NpapiPlugin::browserFuncs.setexception(nullptr, stream.str().c_str());
			return false;
		}
	}
};

template <typename CalleeType, CalleeType callee>
struct MethodThunk
{
	typedef typename Meta::Callable<CalleeType>::Args Args;
	typedef typename Meta::Callable<CalleeType>::ReturnType ReturnType;

	struct Helper
	{
		static inline ReturnType Call(const Args& args)
		{
			return Meta::Callable<CalleeType>::Call(callee, args);
		}
	};

	static inline bool Thunk(NPObject* thisObject, const NPVariant* args, int argsCount, NPVariant* result)
	{
		try
		{
			ArgGettingState state(args, argsCount, true, thisObject);
			CallAndReturn<Helper, ReturnType, Args>::Do(Args(state), result);
			return true;
		}
		catch(Exception* exception)
		{
			std::ostringstream stream;
			MakePointer(exception)->PrintStack(stream);
			Platform::NpapiPlugin::browserFuncs.setexception(nullptr, stream.str().c_str());
			return false;
		}
	}
};

template <typename CalleeType>
struct ConstructorThunk
{
	typedef typename Meta::CallableConstructor<CalleeType>::ClassType ClassType;
	typedef typename Meta::CallableConstructor<CalleeType>::Args Args;
	typedef typename Meta::CallableConstructor<CalleeType>::ReturnType ReturnType;

	static inline bool Thunk(const NPVariant* args, int argsCount, NPVariant* result)
	{
		try
		{
			ArgGettingState argGettingState(args, argsCount, false);
			*result = Value<ReturnType>::To(Meta::CallableConstructor<CalleeType>::Call(Args(argGettingState)));
			return true;
		}
		catch(Exception* exception)
		{
			std::ostringstream stream;
			stream << Meta::MetaOf<MetaProvider, ClassType>()->GetFullName();
			stream << " instance constructor failed:\n";
			MakePointer(exception)->PrintStack(stream);
			Platform::NpapiPlugin::browserFuncs.setexception(nullptr, stream.str().c_str());
			return false;
		}
	}
};

END_INANITY_NP

#endif
