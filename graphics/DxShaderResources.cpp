#include "DxShaderResources.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"

BEGIN_INANITY_GRAPHICS

DxShaderResources::DxShaderResources(int uniformBuffersMask, int samplersMask)
: uniformBuffersMask(uniformBuffersMask), samplersMask(samplersMask) {}

void DxShaderResources::Serialize(StreamWriter& writer)
{
	writer.WriteShortly(uniformBuffersMask);
	writer.WriteShortly(samplersMask);
}

DxShaderResources DxShaderResources::Deserialize(StreamReader& reader)
{
	int uniformBuffersMask = (int)reader.ReadShortly();
	int samplersMask = (int)reader.ReadShortly();
	return DxShaderResources(uniformBuffersMask, samplersMask);
}

END_INANITY_GRAPHICS
