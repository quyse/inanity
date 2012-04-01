#include "ShaderSource.hpp"
#include "../File.hpp"
#include "../FileSystem.hpp"

ShaderSource::ShaderSource(ptr<File> code, ptr<FileSystem> includes, const String& functionName, const String& profile, const std::vector<String>& parameters)
: code(code), includes(includes), functionName(functionName), profile(profile), parameters(parameters)
{
}

ptr<File> ShaderSource::GetCode() const
{
	return code;
}

ptr<FileSystem> ShaderSource::GetIncludes() const
{
	return includes;
}

const String& ShaderSource::GetFunctionName() const
{
	return functionName;
}

const String& ShaderSource::GetProfile() const
{
	return profile;
}

const std::vector<String>& ShaderSource::GetParameters() const
{
	return parameters;
}
