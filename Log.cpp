#include "Log.hpp"
#include "Exception.hpp"
#include <iostream>
#include <sstream>
#ifdef ___INANITY_PLATFORM_WINDOWS
#include "platform/windows.hpp"
#endif

BEGIN_INANITY

//*** class Log

Log* Log::instance = &StandardLog::instance;

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

void Log::Error(ptr<Exception> exception)
{
	std::ostringstream stream;
	exception->PrintStack(stream);
	Error(stream.str());
}

//*** class StandardLog

StandardLog StandardLog::instance;

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
	std::cerr << severities[severity] << string << '\n';
}

END_INANITY
