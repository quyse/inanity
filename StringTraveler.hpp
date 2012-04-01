#ifndef ___INANITY_STRING_TRAVELER_HPP___
#define ___INANITY_STRING_TRAVELER_HPP___

#include "String.hpp"

BEGIN_INANITY

/// Вспомогательный класс для прохождения по UTF-8 строке, и получения символов.
class StringTraveler
{
private:
	const String& string;
	String::const_iterator position;

public:
	StringTraveler(const String& string);

	bool Next(wchar_t& symbol);
};

END_INANITY

#endif
