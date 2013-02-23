#include "HlslSource.hpp"
#include "../File.hpp"
#include "../FileSystem.hpp"
#include "../OutputStream.hpp"
#include "../StreamWriter.hpp"

HlslSource::HlslSource(ptr<File> code, const String& functionName, const String& profile, ptr<FileSystem> includes)
: code(code), functionName(functionName), profile(profile), includes(includes) {}

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

ptr<FileSystem> HlslSource::GetIncludes() const
{
	return includes;
}

void HlslSource::Serialize(ptr<OutputStream> outputStream)
{
	StreamWriter writer(outputStream);

	writer.WriteString(functionName);
	writer.WriteString(profile);
	outputStream->WriteFile(code);
}
