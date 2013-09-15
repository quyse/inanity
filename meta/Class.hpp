#ifndef ___INANITY_META_CLASS_HPP___
#define ___INANITY_META_CLASS_HPP___

#include "Extension.hpp"
#include <vector>

#include "../script/lua/extension.hpp"
#include "../script/v8/extension.hpp"

BEGIN_INANITY_META

class ConstructorBase;
class MethodBase;
class FunctionBase;

/// Base non-templated class.
class ClassBase
{
public:
	typedef std::vector<MethodBase*> Methods;
	typedef std::vector<FunctionBase*> StaticMethods;

protected:
	/// Name of the class.
	const char* name;
	/// Full name of the class (with namespaces).
	const char* fullName;

	ClassBase* parent;

	ConstructorBase* constructor;
	Methods methods;
	StaticMethods staticMethods;

	ClassBase(const char* name, const char* fullName);

public:
	const char* GetName() const;
	const char* GetFullName() const;

	void SetParent(ClassBase* parent);
	ClassBase* GetParent() const;

	void SetConstructor(ConstructorBase* constructor);
	void AddMethod(MethodBase* method);
	void AddStaticMethod(FunctionBase* staticMethod);

	ConstructorBase* GetConstructor() const;
	const Methods& GetMethods() const;
	const StaticMethods& GetStaticMethods() const;

	virtual Script::Lua::ClassExtensionBase* GetLuaExtension() = 0;
	virtual Script::V8::ClassExtensionBase* GetV8Extension() = 0;
};

template <typename ClassType>
class Class;

END_INANITY_META

#endif
