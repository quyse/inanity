#ifndef ___INANITY_SCRIPT_V8_THUNKS_HPP___
#define ___INANITY_SCRIPT_V8_THUNKS_HPP___

#include "v8.hpp"

BEGIN_INANITY_V8

template <typename CalleeType, CalleeType callee>
struct CalleeThunk;

template <typename CalleeType>
struct ConstructorThunk;

END_INANITY_V8

#endif
