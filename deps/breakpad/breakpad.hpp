#ifndef ___INANITY_BREAKPAD_HPP___
#define ___INANITY_BREAKPAD_HPP___

#include "../../Object.hpp"
#include <unordered_map>
#include <string>
#if defined(___INANITY_PLATFORM_WINDOWS)
#include "../../platform/windows.hpp"
#endif

BEGIN_INANITY

struct BreakpadInfo
{
	std::string appName;
#if defined(___INANITY_PLATFORM_WINDOWS)
	PCWSTR appIcon;
#endif
	const char* crashSendUrl;
	std::unordered_map<std::string, std::string> crashSendParameters;
};

void setupBreakpadExceptionHandler(BreakpadInfo&& info);

END_INANITY

#endif
