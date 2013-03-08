#ifndef ___INANITY_GRAPHICS_PNG_IMAGE_LOADER_HPP___
#define ___INANITY_GRAPHICS_PNG_IMAGE_LOADER_HPP___

#include "ImageLoader.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс загрузчика PNG-изображений.
class PngImageLoader : public ImageLoader
{
private:
	/// Вспомогательный класс.
	class Helper;

public:
	ptr<Image2DData> Load(ptr<File> file);
};

END_INANITY_GRAPHICS

#endif
