#ifndef ___INANITY_META_CLASS_HPP___
#define ___INANITY_META_CLASS_HPP___

#include "Extension.hpp"
#include "Constructor.hpp"
#include "Function.hpp"
#include "Method.hpp"
#include <vector>

BEGIN_INANITY_META

/// Base non-templated class.
class ClassBase
{
public:
	typedef std::vector<ConstructorBase*> Constructors;
	typedef std::vector<MethodBase*> Methods;
	typedef std::vector<FunctionBase*> StaticMethods;

protected:
	/// Name of the class.
	const char* name;
	/// Full name of the class (with namespaces).
	const char* fullName;

	ClassBase* parent;

	Constructors constructors;
	Methods methods;
	StaticMethods staticMethods;

	ClassBase(const char* name, const char* fullName, ClassBase* parent);

public:
	const char* GetName() const;
	const char* GetFullName() const;

	void SetParent(ClassBase* parent) const;
	ClassBase* GetParent() const;

	void AddConstructor(ConstructorBase* constructor);
	void AddMethod(MethodBase* method);
	void AddStaticMethod(FunctionBase* staticMethod);

	const Constructors& GetConstructors() const;
	const Methods& GetMethods() const;
	const StaticMethods& GetStaticMethods() const;
};

/// Stores metainformation about C++ class.
/** Members of ParentClassType are inherited by derived class.
If there is no parent class, it should be void. */
template <typename ClassType>
class Class : public ClassBase
{
public:
	typedef Extension<Class<ClassType> > ExtensionType;

private:
	ExtensionType extension;

public:
	Class(const char* name, const char* fullName);

	const ExtensionType& GetExtension() const;
};

/// Initializes a class metainformation.
/** Doesn't have generic implementation. Concrete specializations
initialize concrete classes. */
template <typename ClassType>
void ClassInitialize(Class<ClassType>& meta);

END_INANITY_META

#endif
