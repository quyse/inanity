#ifndef ___INANITY_GRAPHICS_FONT_MANAGER_HPP___
#define ___INANITY_GRAPHICS_FONT_MANAGER_HPP___

#include "graphics.hpp"
#include "../ResourceManager.hpp"

BEGIN_INANITY_GRAPHICS

class Font;
class TextureManager;

/// Класс менеджера шрифтов.
class FontManager : public ResourceManager<Font>
{
private:
	ptr<TextureManager> textureManager;

	ptr<Font> Load(const String& fontName);

public:
	FontManager(ptr<FileSystem> fileSystem, ptr<TextureManager> textureManager);
};

END_INANITY_GRAPHICS

#endif
