#include "global.hpp"

#ifdef ___INANITY_PROFILING

#include "Profiler.hpp"
#include "Reporter.hpp"
#include "../Time.hpp"

BEGIN_INANITY_PROFILE

#ifdef _MSC_VER
#define THREAD_LOCAL __declspec(thread)
#else
#define THREAD_LOCAL thread_local
#endif

//*** Thread-local variables.
THREAD_LOCAL bool g_enable = false;
THREAD_LOCAL Profiler::Record* g_records = 0;
THREAD_LOCAL Profiler::Record* g_recordsEnd = 0;
THREAD_LOCAL Profiler::Chunk* g_chunk = 0;

Profiler g_profiler;

void Issue(RecordType type, const void* note)
{
	if(g_enable)
	{
		if(g_records >= g_recordsEnd)
			g_profiler.FlushChunk(g_chunk, g_records, g_recordsEnd);

		g_records->time = Inanity::Time::GetTicks();
		g_records->type = type;
		g_records->note = note;
		g_records++;
	}
}

void Start()
{
	if(!g_enable)
	{
		g_enable = true;
		// Issue a record only if there are some records before.
		// Otherwise there is no need in this record.
		// There is one more reason to skip adding first record.
		// Profiler::FlushChunk would add stop-resume records
		// before that "manual start" record, and this is strange.
		if(g_records)
			Issue(recordTypeStart, "Manual start");
	}
}

void Stop()
{
	if(g_enable)
	{
		Issue(recordTypeStop, "Manual stop");
		g_enable = false;
	}
}

void Report(std::ostream& stream)
{
	Reporter reporter(g_profiler.GetThreadChunkIterator(), stream);
	reporter.Report();
}

END_INANITY_PROFILE

#endif
