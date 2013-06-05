#ifndef ___INANITY_META_FUNCTION_HPP___
#define ___INANITY_META_FUNCTION_HPP___

#include "Extension.hpp"

BEGIN_INANITY_META

class FunctionBase
{
protected:
	const char* name;

	FunctionBase(const char* name);

public:
	const char* GetName() const;
};

template <typename FunctionType, FunctionType function>
class Function : public FunctionBase
{
public:
	typedef Extension<Function<FunctionType, function> > ExtensionType;

private:
	ExtensionType extension;

public:
	Function(const char* name);

	const ExtensionType& GetExtension() const;
};

END_INANITY_META

#endif
