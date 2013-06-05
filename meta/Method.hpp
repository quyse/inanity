#ifndef ___INANITY_META_METHOD_HPP___
#define ___INANITY_META_METHOD_HPP___

#include "Extension.hpp"

BEGIN_INANITY_META

class MethodBase
{
protected:
	const char* name;

	MethodBase(const char* name);

public:
	const char* GetName() const;
};

template <typename MethodType, MethodType method>
class Method : public MethodBase
{
public:
	typedef Extension<Method<MethodType, method> > ExtensionType;

private:
	ExtensionType extension;

public:
	Method(const char* name);

	const ExtensionType& GetExtension() const;
};

END_INANITY_META

#endif
