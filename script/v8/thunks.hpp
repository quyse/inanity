#ifndef ___INANITY_SCRIPT_V8_THUNKS_HPP___
#define ___INANITY_SCRIPT_V8_THUNKS_HPP___

#include "v8.hpp"

BEGIN_INANITY_V8

template <typename CalleeType, CalleeType callee>
struct CalleeThunk;

template <typename ClassType>
void DummyConstructorThunk(const v8::FunctionCallbackInfo<v8::Value>& info);

template <typename ClassType, typename... Args>
struct ConstructorThunk;

END_INANITY_V8

#endif
