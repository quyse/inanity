#include "breakpad.hpp"
#if defined(___INANITY_PLATFORM_WINDOWS)
#include "repo/src/client/windows/handler/exception_handler.h"
#include "repo/src/client/windows/sender/crash_report_sender.h"
#include "../../Strings.hpp"
#include <sstream>
#include <commctrl.h>
#endif

BEGIN_INANITY

BreakpadInfo g_info;

#if defined(___INANITY_PLATFORM_WINDOWS)

static bool minidumpCallback(const wchar_t* dumpPath, const wchar_t* minidumpId, void* context, EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion, bool succeeded)
{
	if(!succeeded) return false;

	// get dump file path
	std::wstring dumpPathFile;
	{
		std::wostringstream ss;
		ss << dumpPath << minidumpId << L".dmp";
		dumpPathFile = ss.str();
	}

	// ask user
	{
		TASKDIALOGCONFIG taskConfig;
		memset(&taskConfig, 0, sizeof(taskConfig));
		taskConfig.cbSize = sizeof(taskConfig);
		taskConfig.hInstance = GetModuleHandle(NULL);
		taskConfig.dwFlags = TDF_ENABLE_HYPERLINKS | TDF_ALLOW_DIALOG_CANCELLATION | TDF_USE_COMMAND_LINKS | TDF_EXPAND_FOOTER_AREA;
		taskConfig.dwCommonButtons = TDCBF_CLOSE_BUTTON;
		taskConfig.pszWindowTitle = L"Application crash";
		taskConfig.pszMainIcon = g_info.appIcon;
		std::wstring mainInstruction = Strings::UTF82Unicode(g_info.appName) + L" crashed";
		taskConfig.pszMainInstruction = mainInstruction.c_str();
		taskConfig.pszContent = L"Would you like to report it?";
		const TASKDIALOG_BUTTON buttons[] = {
			{ 1, L"Submit crash report\nto help developers fix this problem" }
		};
		taskConfig.cButtons = sizeof(buttons) / sizeof(buttons[0]);
		taskConfig.pButtons = buttons;
		taskConfig.nDefaultButton = 1;
		std::wstring expandedInfo;
		{
			std::wostringstream ss;
			ss << L"Dump file: <a href=\"dump_file\">" << dumpPathFile << L"</a>";
			expandedInfo = ss.str();
		}
		taskConfig.pszExpandedInformation = expandedInfo.c_str();
		taskConfig.pszCollapsedControlText = L"Report's contents";
		taskConfig.pszExpandedControlText = L"Report's contents:";
		int button;
		if(TaskDialogIndirect(&taskConfig, &button, nullptr, nullptr) != S_OK) return true;
	}

	// send report
	google_breakpad::CrashReportSender sender({});
	std::map<std::wstring, std::wstring> parameters;
	for(const auto& parameter : g_info.crashSendParameters)
		parameters.insert({ Strings::UTF82Unicode(parameter.first), Strings::UTF82Unicode(parameter.second) });
	sender.SendCrashReport(Strings::UTF82Unicode(g_info.crashSendUrl).c_str(), parameters, { { L"dump", dumpPathFile } }, nullptr);

	return true;
}

#endif

void setupBreakpadExceptionHandler(BreakpadInfo&& info)
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	// save info
	g_info = std::move(info);

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
