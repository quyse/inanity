#ifndef ___INANITY_PROFILE_RECORD_TYPE_HPP___
#define ___INANITY_PROFILE_RECORD_TYPE_HPP___

#include "profile.hpp"

#ifdef ___INANITY_PROFILING

BEGIN_INANITY_PROFILE

/// Types of records.
enum RecordType
{
	recordTypeEmpty,
	recordTypeStart,
	recordTypeStop,
	recordTypePoint,
	recordTypeScopeEnter,
	recordTypeScopeLeave
};

END_INANITY_PROFILE

#endif

#endif
