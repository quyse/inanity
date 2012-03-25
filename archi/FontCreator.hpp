#ifndef ___INANITY_ARCHI_FONT_CREATOR_H___
#define ___INANITY_ARCHI_FONT_CREATOR_H___

#include "general.hpp"
#include <string>

class FontCreator : public Object
{
private:
	String textureName;
	float charWidth, charHeight;
	std::hash_map<wchar_t, FontChar> charset;

public:
	FontCreator(const String& textureName, float charWidth, float charHeight);

	void Generate256Symbols();
	void GenerateFromAlphabat(const String& alphabatFileName);
	ptr<File> Save();
};

#endif
