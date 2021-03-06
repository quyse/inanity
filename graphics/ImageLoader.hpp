#ifndef ___INANITY_GRAPHICS_IMAGE_LOADER_HPP___
#define ___INANITY_GRAPHICS_IMAGE_LOADER_HPP___

#include "PixelFormat.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class RawTextureData;

/// Абстрактный класс загрузчика изображений.
class ImageLoader : public Object
{
public:
	virtual ptr<RawTextureData> Load(ptr<File> file) = 0;
};

END_INANITY_GRAPHICS

#endif
