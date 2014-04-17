#include "Log.hpp"
#include "Exception.hpp"
#include <iostream>
#ifdef ___INANITY_PLATFORM_WINDOWS
#include "platform/windows.hpp"
#endif

BEGIN_INANITY

//*** class Log

Log* Log::instance = &StandardLog::cerrInstance;

void Log::SetInstance(Log* log)
{
	Log::instance = log;
}

void Log::Message(const String& string)
{
	instance->Write(severityMessage, string);
}

void Log::Warning(const String& string)
{
	instance->Write(severityWarning, string);
}

void Log::Error(const String& string)
{
	instance->Write(severityError, string);
}

//*** class StandardLog

StandardLog StandardLog::cerrInstance(std::cerr);

StandardLog::StandardLog(std::ostream& stream)
: stream(stream) {}

void StandardLog::Write(Severity severity, const String& string)
{
	static const char* severities[] =
	{
		"[MSG ] ", "[WARN] ", "[ERR ] "
	};

#if defined(_DEBUG) && defined(___INANITY_PLATFORM_WINDOWS)
	if(IsDebuggerPresent())
		OutputDebugStringA((severities[severity] + string + "\n").c_str());
	else
#endif
	stream << severities[severity] << string << '\n';
}

END_INANITY
