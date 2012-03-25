#ifndef ___INANITY_ARCHI_SYSTEM_FONT_CREATOR_H___
#define ___INANITY_ARCHI_SYSTEM_FONT_CREATOR_H___

#include "Processor.hpp"
#include <string>

class SystemFontCreator : public Processor
{
private:
	LOGFONT lf;
	String textureFileName;
	std::unordered_map<wchar_t, FontChar> charset;
	int charHeight;

	void CreateImage(const String& alphabatFileName, const String& textureFileName);

public:

	String GetCommand() const;
	void PrintHelp() const;
	void Run(const std::vector<String>& arguments);
};

#endif
