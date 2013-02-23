#include "GlslSource.hpp"
#include "../File.hpp"
#include "../OutputStream.hpp"

GlslSource::GlslSource(ptr<File> code) : code(code) {}

void GlslSource::Serialize(ptr<OutputStream> outputStream)
{
	outputStream->WriteFile(code);
}

ptr<File> GlslSource::GetCode() const
{
	return code;
}
