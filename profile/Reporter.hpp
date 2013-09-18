#ifndef ___INANITY_PROFILE_REPORTER_HPP___
#define ___INANITY_PROFILE_REPORTER_HPP___

#include "Profiler.hpp"

#ifdef ___INANITY_PROFILING

#include <iostream>

BEGIN_INANITY_PROFILE

/// Processes profile information, and print report.
class Reporter
{
private:
	Profiler::ThreadChunkIterator threadChunkIterator;
	std::ostream& stream;

	void ReportPoints();
	void ReportScopes();

public:
	Reporter(Profiler::ThreadChunkIterator threadChunkIterator, std::ostream& stream);

	void RawReport();
	void Report();
};

END_INANITY_PROFILE

#endif

#endif
