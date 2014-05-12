#ifndef ___INANITY_GRAPHICS_UNIVERSAL_IMAGE_LOADER_HPP___
#define ___INANITY_GRAPHICS_UNIVERSAL_IMAGE_LOADER_HPP___

#include "ImageLoader.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

/// Image loader supporting several image formats.
class UniversalImageLoader : public ImageLoader
{
private:
	std::vector<ptr<ImageLoader> > loaders;

public:
	UniversalImageLoader();
	~UniversalImageLoader();

	//*** ImageLoader's methods.
	ptr<RawTextureData> Load(ptr<File> file);
};

END_INANITY_GRAPHICS

#endif
