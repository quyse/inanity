#ifndef ___INANITY_META_CONSTRUCTOR_HPP___
#define ___INANITY_META_CONSTRUCTOR_HPP___

#include "Extension.hpp"

BEGIN_INANITY_META

class ConstructorBase
{
};

template <typename ConstructorType>
class Constructor : public ConstructorBase
{
public:
	typedef Extension<Constructor<ConstructorType> > ExtensionType;

private:
	ExtensionType extension;

public:
	const ExtensionType& GetExtension() const;
};

END_INANITY_META

#endif
