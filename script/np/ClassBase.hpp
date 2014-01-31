#ifndef ___INANITY_SCRIPT_NP_CLASS_BASE_HPP___
#define ___INANITY_SCRIPT_NP_CLASS_BASE_HPP___

#include "np.hpp"
#include "MetaProvider.hpp"
#include "wrappers.hpp"
#include "../../meta/ClassBase.hpp"
#include <unordered_map>

BEGIN_INANITY_NP

class ClassBase : public Meta::ClassBase<MetaProvider::Traits>
{
public:
	typedef std::unordered_map<NPIdentifier, int> IdentifierMap;

protected:
	NPClassWrapper npClassWrapper;
	/// Map of method indices by NP identifiers.
	IdentifierMap methodsByIdentifier;
	/// Map of static method indices by NP identifiers.
	IdentifierMap staticMethodsByIdentifier;

public:
	ClassBase(const char* name, const char* fullName);

	void AddMethod(MethodBase* method);
	void AddStaticMethod(FunctionBase* staticMethod);

	NPClassWrapper* GetClassWrapper();
	const IdentifierMap& GetMethodsByIdentifier() const;
	const IdentifierMap& GetStaticMethodsByIdentifier() const;
};

END_INANITY_NP

#endif
