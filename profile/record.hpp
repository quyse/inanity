#ifndef ___INANITY_PROFILE_RECORD_HPP___
#define ___INANITY_PROFILE_RECORD_HPP___

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

/// Additional record data in case of scopes or points.
struct RecordNote
{
	/// Name of a file.
	const char* file;
	/// Line number.
	int line;
	/// Name of a function.
	const char* function;
	/// Description.
	const char* description;
};

END_INANITY_PROFILE

#endif

#endif
