#include "Dx11CompiledShader.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"
#include "../MemoryFile.hpp"

BEGIN_INANITY_GRAPHICS

Dx11CompiledShader::Dx11CompiledShader(ptr<File> code, const Dx11ShaderResources& resources)
: code(code), resources(resources) {}

ptr<File> Dx11CompiledShader::GetCode() const
{
	return code;
}

const Dx11ShaderResources& Dx11CompiledShader::GetResources() const
{
	return resources;
}

void Dx11CompiledShader::Serialize(ptr<OutputStream> outputStream)
{
	StreamWriter writer(outputStream);

	writer.WriteShortly(code->GetSize());
	writer.Write(code);

	resources.Serialize(writer);
}

ptr<Dx11CompiledShader> Dx11CompiledShader::Deserialize(ptr<InputStream> inputStream)
{
	StreamReader reader(inputStream);

	size_t codeSize = reader.ReadShortly();
	ptr<File> code = NEW(MemoryFile(codeSize));
	reader.Read(code->GetData(), codeSize);

	Dx11ShaderResources resources = Dx11ShaderResources::Deserialize(reader);

	return NEW(Dx11CompiledShader(code, resources));
}

END_INANITY_GRAPHICS
