#ifndef ___INANITY_GRAPHICS_TEXTURE_MANAGER_HPP___
#define ___INANITY_GRAPHICS_TEXTURE_MANAGER_HPP___

#include "graphics.hpp"
#include "../ResourceManager.hpp"

BEGIN_INANITY_GRAPHICS

class Device;
class Texture;

/// Класс менеджера текстур.
class TextureManager : public ResourceManager<Texture>
{
private:
	ptr<Device> device;

	ptr<Texture> Load(const String& textureName);

public:
	TextureManager(ptr<FileSystem> fileSystem, ptr<Device> device);
};

END_INANITY_GRAPHICS

#endif
