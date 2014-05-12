#include "UniversalImageLoader.hpp"
#include "RawTextureData.hpp"
#include "PngImageLoader.hpp"
#include "TgaImageLoader.hpp"
#include "BmpImage.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

UniversalImageLoader::UniversalImageLoader()
{
	loaders.push_back(BmpImage::CreateLoader());
	loaders.push_back(NEW(PngImageLoader()));
	loaders.push_back(NEW(TgaImageLoader()));
}

UniversalImageLoader::~UniversalImageLoader() {}

ptr<RawTextureData> UniversalImageLoader::Load(ptr<File> file)
{
	for(size_t i = 0; i < loaders.size(); ++i)
		try
		{
			return loaders[i]->Load(file);
		}
		catch(Exception* exception)
		{
			MakePointer(exception);
		}

	THROW("Can't load image via universal image loader");
}

END_INANITY_GRAPHICS
