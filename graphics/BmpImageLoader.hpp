#ifndef ___INANITY_GRAPHICS_BMP_IMAGE_LOADER_HPP___
#define ___INANITY_GRAPHICS_BMP_IMAGE_LOADER_HPP___

#include "ImageLoader.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс загрузчика BMP-изображений.
class BmpImageLoader : public ImageLoader
{
private:
	struct FileHeader;
	struct InfoHeader;

public:
	ptr<RawTextureData> Load(ptr<File> file);
};

END_INANITY_GRAPHICS

#endif
