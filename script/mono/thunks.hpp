#ifndef ___INANITY_SCRIPT_MONO_THUNKS_HPP___
#define ___INANITY_SCRIPT_MONO_THUNKS_HPP___

BEGIN_INANITY_MONO

template <typename ClassType, typename... Args>
struct ConstructorHelper;

template <typename FunctionType, FunctionType function>
struct FunctionHelper;

template <typename MethodType, MethodType method>
struct MethodHelper;

END_INANITY_MONO

#endif
