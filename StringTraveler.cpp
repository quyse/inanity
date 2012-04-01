#include "StringTraveler.hpp"
#include "utf8.h"

StringTraveler::StringTraveler(const String& string) : string(string), position(string.begin())
{
}

bool StringTraveler::Next(wchar_t& symbol)
{
	if(position >= string.end())
		return false;

	symbol = (wchar_t)utf8::unchecked::next(position);
	return true;
}
