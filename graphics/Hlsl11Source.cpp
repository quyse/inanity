#include "Hlsl11Source.hpp"
#include "../File.hpp"
#include "../OutputStream.hpp"
#include "../StreamWriter.hpp"

BEGIN_INANITY_GRAPHICS

Hlsl11Source::Hlsl11Source(ptr<File> code, const String& functionName, const String& profile, const Dx11ShaderResources& resources)
: code(code), functionName(functionName), profile(profile), resources(resources) {}

ptr<File> Hlsl11Source::GetCode() const
{
	return code;
}

const String& Hlsl11Source::GetFunctionName() const
{
	return functionName;
}

const String& Hlsl11Source::GetProfile() const
{
	return profile;
}

const Dx11ShaderResources& Hlsl11Source::GetResources() const
{
	return resources;
}

void Hlsl11Source::Serialize(ptr<OutputStream> outputStream)
{
	StreamWriter writer(outputStream);

	writer.WriteString(functionName);
	writer.WriteString(profile);
	outputStream->Write(code);
	resources.Serialize(writer);
}

END_INANITY_GRAPHICS
