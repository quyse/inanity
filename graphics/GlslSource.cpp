#include "GlslSource.hpp"
#include "GlShaderBindings.hpp"
#include "../MemoryFile.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"

GlslSource::GlslSource(ptr<File> code, ptr<GlShaderBindings> shaderBindings)
: code(code), shaderBindings(shaderBindings) {}

void GlslSource::Serialize(ptr<OutputStream> outputStream)
{
	StreamWriter writer(outputStream);

	// записать код
	writer.WriteShortly(code->GetSize());
	writer.WriteFile(code);
	// записать привязки
	shaderBindings->Serialize(writer);
}

ptr<GlslSource> GlslSource::Deserialize(ptr<InputStream> inputStream)
{
	StreamReader reader(inputStream);

	// считать код
	size_t codeSize = reader.ReadShortly();
	ptr<File> code = NEW(MemoryFile(codeSize));
	reader.Read(code->GetData(), codeSize);
	// считать привязки
	ptr<GlShaderBindings> shaderBindings = GlShaderBindings::Deserialize(reader);

	return NEW(GlslSource(code, shaderBindings));
}

ptr<File> GlslSource::GetCode() const
{
	return code;
}

ptr<GlShaderBindings> GlslSource::GetShaderBindings() const
{
	return shaderBindings;
}
