#ifndef ___INANITY_SCRIPT_V8_EXTENSION_HPP___
#define ___INANITY_SCRIPT_V8_EXTENSION_HPP___

#include "v8.hpp"
#include "../../meta/extension_decl.hpp"
#include "v8lib.hpp"

BEGIN_INANITY_V8

template <typename Type>
class Extension;

class ClassExtensionBase
{
};

/// Class meta extension for V8.
template <typename ClassType>
class Extension<Meta::Class<ClassType> > : public ClassExtensionBase
{
};

class CallableExtensionBase
{
public:
	virtual v8::FunctionCallback GetThunk() const = 0;
};

typedef CallableExtensionBase ConstructorExtensionBase;

template <typename ConstructorType>
class Extension<Meta::Constructor<ConstructorType> > : public ConstructorExtensionBase
{
public:
};

typedef CallableExtensionBase FunctionExtensionBase;

template <typename FunctionType, FunctionType function>
class Extension<Meta::Function<FunctionType, function> > : public FunctionExtensionBase
{
public:
};

typedef CallableExtensionBase MethodExtensionBase;

template <typename MethodType, MethodType method>
class Extension<Meta::Method<MethodType, method> > : public MethodExtensionBase
{
public:
};

END_INANITY_V8

#endif
