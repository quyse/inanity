#ifndef ___INANITY_META_OF_HPP___
#define ___INANITY_META_OF_HPP___

#include "meta.hpp"
#include "Class.hpp"
#include "Constructor.hpp"
#include "Function.hpp"
#include "Method.hpp"

BEGIN_INANITY_META

template <typename ClassType>
struct OfClass;

template <typename ConstructorType>
struct OfConstructor;

template <typename MethodType, MethodType method>
struct OfMethod;

template <typename ClassType, typename MethodType, MethodType method>
struct OfStaticMethod;

END_INANITY_META

#endif
