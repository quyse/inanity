#ifndef ___INANITY_GRAPHICS_DX11_SHADER_RESOURCES_HPP___
#define ___INANITY_GRAPHICS_DX11_SHADER_RESOURCES_HPP___

#include "graphics.hpp"

BEGIN_INANITY

class StreamWriter;
class StreamReader;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Структура, указывающая ресурсы, необходимые DirectX-шейдеру.
struct Dx11ShaderResources
{
	/// Маска используемых uniform-буферов.
	int uniformBuffersMask;
	/// Маска используемых семплеров.
	int samplersMask;

	Dx11ShaderResources(int uniformBuffersMask = 0, int samplersMask = 0);

	void Serialize(StreamWriter& writer);
	static Dx11ShaderResources Deserialize(StreamReader& reader);
};

END_INANITY_GRAPHICS

#endif
