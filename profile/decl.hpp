#ifndef ___INANITY_PROFILE_DECL_HPP___
#define ___INANITY_PROFILE_DECL_HPP___

/* Inclusion of this file is enough to profile things. */

#include "profile.hpp"

#ifdef ___INANITY_PROFILING

#include "global.hpp"
#include "Scope.hpp"
#include "../Exception.hpp"

/// Code position.
#define PROFILE_CODE_POSITION \
	__FILE__ ":" __SLINE__

/// Profile some point in code.
/** Counts a number of executions of this point. */
#define PROFILE_POINT() \
	Inanity::Profile::Issue(Inanity::Profile::recordTypePoint, PROFILE_CODE_POSITION)

/// Profile scope.
#define PROFILE_SCOPE() \
	Inanity::Profile::Scope profileScope(PROFILE_CODE_POSITION)

/// Profile function scope.
/** Uses name of the function instead of code position. */
#define PROFILE_FUNCTION() \
	Inanity::Profile::Scope profileFunctionScope(__FUNCTION__)

#else // ___INANITY_PROFILING

// Dummy macros.

#define PROFILE_POINT()
#define PROFILE_SCOPE()
#define PROFILE_FUNCTION()

#endif // ___INANITY_PROFILING

#endif
