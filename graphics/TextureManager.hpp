#ifndef ___INANITY_GRAPHICS_TEXTURE_MANAGER_HPP___
#define ___INANITY_GRAPHICS_TEXTURE_MANAGER_HPP___

#include "graphics.hpp"
#include "SamplerSettings.hpp"
#include "../ResourceManager.hpp"

BEGIN_INANITY_GRAPHICS

class Device;
class Texture;
class ImageLoader;

/// Класс менеджера текстур.
class TextureManager : public ResourceManager<Texture>
{
private:
	ptr<Device> device;
	SamplerSettings samplerSettings;

	/// Загрузчики изображений по расширениям обрабатываемых файлов.
	std::unordered_map<String, ptr<ImageLoader> > imageLoaders;

	ptr<Texture> Load(const String& textureName);

public:
	TextureManager(ptr<FileSystem> fileSystem, ptr<Device> device, const SamplerSettings& samplerSettings);
};

END_INANITY_GRAPHICS

#endif
