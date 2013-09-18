#ifndef ___INANITY_PROFILE_GLOBAL_HPP___
#define ___INANITY_PROFILE_GLOBAL_HPP___

#include "RecordType.hpp"

#ifdef ___INANITY_PROFILING

BEGIN_INANITY_PROFILE

void Issue(RecordType type, const char* note);
void Start();
void Stop();
void Report(std::ostream& stream);

END_INANITY_PROFILE

#endif

#endif
