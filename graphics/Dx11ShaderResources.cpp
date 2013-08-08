#include "Dx11ShaderResources.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"

BEGIN_INANITY_GRAPHICS

Dx11ShaderResources::Dx11ShaderResources(int uniformBuffersMask, int samplersMask)
: uniformBuffersMask(uniformBuffersMask), samplersMask(samplersMask) {}

void Dx11ShaderResources::Serialize(StreamWriter& writer)
{
	writer.WriteShortly(uniformBuffersMask);
	writer.WriteShortly(samplersMask);
}

Dx11ShaderResources Dx11ShaderResources::Deserialize(StreamReader& reader)
{
	int uniformBuffersMask = (int)reader.ReadShortly();
	int samplersMask = (int)reader.ReadShortly();
	return Dx11ShaderResources(uniformBuffersMask, samplersMask);
}

END_INANITY_GRAPHICS
