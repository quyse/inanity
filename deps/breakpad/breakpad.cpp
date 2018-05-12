#include "breakpad.hpp"
#if defined(___INANITY_PLATFORM_WINDOWS)
#include "repo/src/client/windows/handler/exception_handler.h"
#include "repo/src/client/windows/sender/crash_report_sender.h"
#include "../../Strings.hpp"
#include <sstream>
#endif

BEGIN_INANITY

#if defined(___INANITY_PLATFORM_WINDOWS)

static std::wstring g_crashSenderUrl;
static std::map<std::wstring, std::wstring> g_parameters;

static bool minidumpCallback(const wchar_t* dumpPath, const wchar_t* minidumpId, void* context, EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion, bool succeeded)
{
	// get dump file path
	std::wostringstream ss;
	ss << dumpPath << L'\\' << minidumpId << L".dmp";

	// send report
	google_breakpad::CrashReportSender sender({});
	sender.SendCrashReport(g_crashSenderUrl.c_str(), g_parameters, { { L"dump", ss.str() } }, nullptr);

	return succeeded;
}

#endif

void setupBreakpadExceptionHandler(const char* url, std::unordered_map<std::string, std::string>&& parameters)
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	// save url and parameters
	g_crashSenderUrl = Strings::UTF82Unicode(url);
	for(auto&& parameter : parameters)
		g_parameters.insert({ Strings::UTF82Unicode(parameter.first), Strings::UTF82Unicode(parameter.second) });

	// get temp path for storing dumps
	wchar_t tempPath[MAX_PATH + 1];
	if(!GetTempPath(MAX_PATH + 1, tempPath))
	{
		tempPath[0] = 0;
	}
	// register exception handler
	new google_breakpad::ExceptionHandler(tempPath, nullptr, &minidumpCallback, nullptr, google_breakpad::ExceptionHandler::HANDLER_ALL);
#endif
}

END_INANITY
