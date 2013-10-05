#ifndef ___INANITY_META_CLASS_BASE_HPP___
#define ___INANITY_META_CLASS_BASE_HPP___

#include "meta.hpp"
#include <vector>

BEGIN_INANITY_META

/// Foundation class for writing class bases.
template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
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
};

/// Metainformation of class.
template <typename Provider, typename ClassType>
class Class;

/// Get metainformation of class with specified provider.
template <typename Provider, typename ClassType>
typename Provider::ClassBase* MetaOf();

/// Converter of value-type things from script.
template <typename Provider, typename Type>
struct ConverterFromScript;

template <typename Provider, typename Type>
Type ConvertFromScript(const typename Provider::Any& value)
{
	return ConverterFromScript<Provider, Type>::Convert(value);
}

END_INANITY_META

#endif
