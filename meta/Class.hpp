#ifndef ___INANITY_META_CLASS_HPP___
#define ___INANITY_META_CLASS_HPP___

#include "Extension.hpp"
#include <vector>

#ifdef ___INANITY_META_LUA___
#include "../script/lua/extension.hpp"
#endif

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

#ifdef ___INANITY_META_LUA___
	virtual Script::Lua::ClassExtensionBase* GetLuaExtension() = 0;
#endif
};

/// Stores metainformation about C++ class.
/** Members of ParentClassType are inherited by derived class.
If there is no parent class, it should be void. */
template <typename ClassType>
class Class : public ClassBase
{
#ifdef ___INANITY_META_LUA___
private:
	Script::Lua::Extension<Class<ClassType> > luaExtension;
public:
	Script::Lua::ClassExtensionBase* GetLuaExtension();
#endif

public:
	Class(const char* name, const char* fullName);
};

END_INANITY_META

#endif
