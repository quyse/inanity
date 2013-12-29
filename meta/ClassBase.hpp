#ifndef ___INANITY_META_CLASS_BASE_HPP___
#define ___INANITY_META_CLASS_BASE_HPP___

#include "meta.hpp"
#include <vector>

BEGIN_INANITY_META

/// Foundation class for writing class bases.
/** Traits should contain:
ClassBase, ConstructorBase, FunctionBase, MethodBase.
*/
template <typename Traits>
class ClassBase
{
public:
	typedef typename Traits::ConstructorBase ConstructorBase;
	typedef typename Traits::FunctionBase FunctionBase;
	typedef typename Traits::MethodBase MethodBase;
	typedef typename Traits::ClassBase ParentClassBase;

	typedef std::vector<MethodBase*> Methods;
	typedef std::vector<FunctionBase*> StaticMethods;

protected:
	/// Name of the class.
	const char* name;
	/// Full name of the class (with namespaces).
	const char* fullName;

	ParentClassBase* parent;

	ConstructorBase* constructor;
	Methods methods;
	StaticMethods staticMethods;

	ClassBase(const char* name, const char* fullName);

public:
	const char* GetName() const;

	const char* GetFullName() const;

	void SetParent(ParentClassBase* parent);

	ParentClassBase* GetParent() const;

	void SetConstructor(ConstructorBase* constructor);
	void AddMethod(MethodBase* method);
	void AddStaticMethod(FunctionBase* staticMethod);

	ConstructorBase* GetConstructor() const;
	const Methods& GetMethods() const;
	const StaticMethods& GetStaticMethods() const;

	/// Check that object of this class could be
	/// casted to target class.
	bool CanCastTo(ClassBase* targetClass) const;
};

/// Metainformation of class.
template <typename Provider, typename ClassType>
class Class;

/// Get metainformation of class with specified provider.
template <typename Provider, typename ClassType>
typename Provider::ClassBase* MetaOf();

END_INANITY_META

#endif
