#include "TempFileSystem.hpp"
#include "File.hpp"

BEGIN_INANITY

String TempFileSystem::staticResourceName = "/inanity/tempfs";

ptr<File> TempFileSystem::TryLoadFile(const String& fileName)
{
	std::unordered_map<String, ptr<File> >::const_iterator i = files.find(fileName);
	return (i == files.end()) ? nullptr : i->second;
}

void TempFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	files[fileName] = file;
}

ptr<TempFileSystem> TempFileSystem::CreateStaticResource(ResourceLoader* resourceLoader)
{
	return NEW(TempFileSystem());
}

END_INANITY
