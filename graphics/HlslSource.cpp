#include "HlslSource.hpp"
#include "../File.hpp"
#include "../OutputStream.hpp"
#include "../StreamWriter.hpp"

BEGIN_INANITY_GRAPHICS

HlslSource::HlslSource(ptr<File> code, const String& functionName, const String& profile)
: code(code), functionName(functionName), profile(profile) {}

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

void HlslSource::Serialize(ptr<OutputStream> outputStream)
{
	StreamWriter writer(outputStream);

	writer.WriteString(functionName);
	writer.WriteString(profile);
	outputStream->WriteFile(code);
}

END_INANITY_GRAPHICS
