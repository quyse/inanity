#include "DxCompiledShader.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"
#include "../MemoryFile.hpp"

BEGIN_INANITY_GRAPHICS

DxCompiledShader::DxCompiledShader(ptr<File> code, const DxShaderResources& resources)
: code(code), resources(resources) {}

ptr<File> DxCompiledShader::GetCode() const
{
	return code;
}

const DxShaderResources& DxCompiledShader::GetResources() const
{
	return resources;
}

void DxCompiledShader::Serialize(ptr<OutputStream> outputStream)
{
	StreamWriter writer(outputStream);

	writer.WriteShortly(code->GetSize());
	writer.WriteFile(code);

	resources.Serialize(writer);
}

ptr<DxCompiledShader> DxCompiledShader::Deserialize(ptr<InputStream> inputStream)
{
	StreamReader reader(inputStream);

	size_t codeSize = reader.ReadShortly();
	ptr<File> code = NEW(MemoryFile(codeSize));
	reader.Read(code->GetData(), codeSize);

	DxShaderResources resources = DxShaderResources::Deserialize(reader);

	return NEW(DxCompiledShader(code, resources));
}

END_INANITY_GRAPHICS
