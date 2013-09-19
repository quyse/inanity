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
#define PROFILE_POINT(name) \
	{ \
		static const Inanity::Profile::RecordNote note = { __FILE__, __LINE__, __FUNCTION__, name }; \
		Inanity::Profile::Issue(Inanity::Profile::recordTypePoint, &note); \
	}

/// Profile scope.
#define PROFILE_SCOPE(name) \
	static const Inanity::Profile::RecordNote profileScopeNote = { __FILE__, __LINE__, __FUNCTION__, name }; \
	Inanity::Profile::Scope profileScope(&profileScopeNote)

#else // ___INANITY_PROFILING

// Dummy macros.

#define PROFILE_POINT()
#define PROFILE_SCOPE()

#endif // ___INANITY_PROFILING

#endif
