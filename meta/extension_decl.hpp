#ifndef ___INANITY_META_EXTENSION_DECL_HPP___
#define ___INANITY_META_EXTENSION_DECL_HPP___

// The file contains forward declarations for types are needed to
// declare meta extensions.

#include "meta.hpp"

BEGIN_INANITY_META

template <typename ConstructorType>
class Constructor;

template <typename FunctionType, FunctionType function>
class Function;

template <typename MethodType, MethodType method>
class Method;

template <typename ClassType>
class Class;

END_INANITY_META

#endif
