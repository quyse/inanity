#ifndef ___INANITY_GRAPHICS_DX_SHADER_RESOURCES_HPP___
#define ___INANITY_GRAPHICS_DX_SHADER_RESOURCES_HPP___

#include "graphics.hpp"

BEGIN_INANITY

class StreamWriter;
class StreamReader;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Структура, указывающая ресурсы, необходимые DirectX-шейдеру.
struct DxShaderResources
{
	/// Маска используемых uniform-буферов.
	int uniformBuffersMask;
	/// Маска используемых семплеров.
	int samplersMask;

	DxShaderResources(int uniformBuffersMask = 0, int samplersMask = 0);

	void Serialize(StreamWriter& writer);
	static DxShaderResources Deserialize(StreamReader& reader);
};

END_INANITY_GRAPHICS

#endif
