#ifndef ___INANITY_PROFILE_SCOPE_HPP___
#define ___INANITY_PROFILE_SCOPE_HPP___

#include "global.hpp"

#ifdef ___INANITY_PROFILING

BEGIN_INANITY_PROFILE

class Scope
{
private:
	const void* note;

public:
	inline Scope(const void* note) : note(note)
	{
		Issue(recordTypeScopeEnter, note);
	}
	inline ~Scope()
	{
		Issue(recordTypeScopeLeave, note);
	}
};

END_INANITY_PROFILE

#endif

#endif
