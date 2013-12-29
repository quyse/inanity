#ifndef ___INANITY_SCRIPT_NP_THUNKS_HPP___
#define ___INANITY_SCRIPT_NP_THUNKS_HPP___

#include "np.hpp"

BEGIN_INANITY_NP

template <typename CalleeType, CalleeType callee>
struct FunctionThunk;

template <typename CalleeType, CalleeType callee>
struct MethodThunk;

template <typename CalleeType>
struct ConstructorThunk;

END_INANITY_NP

#endif
