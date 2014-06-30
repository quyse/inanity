#ifndef ___INANITY_META_CALLABLE_HPP___
#define ___INANITY_META_CALLABLE_HPP___

#include "meta.hpp"

BEGIN_INANITY_META

/// Structure for calling functions and methods.
/**
Contains:
enum { isMethod = 0/1 }
typedef CalleeType
typedef Args // tuple for args
typedef ReturnType
typedef ClassType (only for methods)
ReturnType Call(CalleeType callee, const Args& args)
*/
template <typename CalleeType>
struct Callable;

/// Structure for calling constructors.
/**
Contains:
typedef Args
typedef ReturnType
typedef ClassType
ptr<ClassType> Call(const Args& args)
*/
template <typename ClassType, typename... Args>
struct CallableConstructor;

END_INANITY_META

#endif
