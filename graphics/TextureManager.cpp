#include "TextureManager.hpp"
#include "Device.hpp"
#include "Texture.hpp"
#include "BmpImageLoader.hpp"
#include "PngImageLoader.hpp"
#include "Image2DData.hpp"
#include "../ResourceManager.ipp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

TextureManager::TextureManager(ptr<FileSystem> fileSystem, ptr<Device> device)
: ResourceManager(fileSystem), device(device)
{
	imageLoaders["bmp"] = NEW(BmpImageLoader());
	imageLoaders["png"] = NEW(PngImageLoader());
}

ptr<Texture> TextureManager::Load(const String& textureName)
{
	try
	{
		// получить расширение файла
		size_t dotPos = textureName.find_last_of('.');
		if(dotPos == std::string::npos)
			// расширения нет
			THROW_PRIMARY_EXCEPTION("No extension in texture name");
		String extension = textureName.substr(dotPos + 1);
		// получить загрузчик
		std::unordered_map<String, ptr<ImageLoader> >::const_iterator i = imageLoaders.find(extension);
		if(i == imageLoaders.end())
			THROW_PRIMARY_EXCEPTION("No image loader for this extension: " + extension);
		ptr<ImageLoader> imageLoader = i->second;

		// загрузить изображение
		ptr<Image2DData> imageData = imageLoader->Load(fileSystem->LoadFile(textureName));

		// создать текстуру
		return device->CreateStatic2DTexture(imageData);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load texture", exception);
	}
}

END_INANITY_GRAPHICS
