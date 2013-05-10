#include "HlslSource.hpp"
#include "../File.hpp"
#include "../OutputStream.hpp"
#include "../StreamWriter.hpp"

BEGIN_INANITY_GRAPHICS

HlslSource::HlslSource(ptr<File> code, const String& functionName, const String& profile, const DxShaderResources& resources)
: code(code), functionName(functionName), profile(profile), resources(resources) {}

ptr<File> HlslSource::GetCode() const
{
	return code;
}

const String& HlslSource::GetFunctionName() const
{
	return functionName;
}

const String& HlslSource::GetProfile() const
{
	return profile;
}

const DxShaderResources& HlslSource::GetResources() const
{
	return resources;
}

void HlslSource::Serialize(ptr<OutputStream> outputStream)
{
	StreamWriter writer(outputStream);

	writer.WriteString(functionName);
	writer.WriteString(profile);
	outputStream->WriteFile(code);
	resources.Serialize(writer);
}

END_INANITY_GRAPHICS
