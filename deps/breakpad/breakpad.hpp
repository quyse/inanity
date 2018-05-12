#ifndef ___INANITY_BREAKPAD_HPP___
#define ___INANITY_BREAKPAD_HPP___

#include "../../Object.hpp"
#include <unordered_map>

BEGIN_INANITY

void setupBreakpadExceptionHandler(const char* url, std::unordered_map<std::string, std::string>&& parameters = {});

END_INANITY

#endif
