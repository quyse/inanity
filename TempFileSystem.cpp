#include "TempFileSystem.hpp"
#include "File.hpp"

BEGIN_INANITY

ptr<File> TempFileSystem::TryLoadFile(const String& fileName)
{
	std::unordered_map<String, ptr<File> >::const_iterator i = files.find(fileName);
	return (i == files.end()) ? nullptr : i->second;
}

void TempFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	files[fileName] = file;
}

END_INANITY
