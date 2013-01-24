#include "TextureManager.hpp"
#include "Device.hpp"
#include "Texture.hpp"
#include "../ResourceManager.ipp"
#include "../File.hpp"

TextureManager::TextureManager(ptr<FileSystem> fileSystem, ptr<Device> device)
: ResourceManager(fileSystem), device(device) {}

ptr<Texture> TextureManager::Load(const String& textureName)
{
	return device->CreateStaticTexture(fileSystem->LoadFile(textureName));
}
