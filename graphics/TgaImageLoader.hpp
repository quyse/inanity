#ifndef ___INANITY_GRAPHICS_TGA_IMAGE_LOADER_HPP___
#define ___INANITY_GRAPHICS_TGA_IMAGE_LOADER_HPP___

#include "ImageLoader.hpp"

BEGIN_INANITY_GRAPHICS

/// TGA images loader.
class TgaImageLoader : public ImageLoader
{
public:
	ptr<RawTextureData> Load(ptr<File> file);
};

END_INANITY_GRAPHICS

#endif
