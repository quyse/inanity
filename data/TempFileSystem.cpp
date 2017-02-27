#include "TempFileSystem.hpp"
#include "../File.hpp"

BEGIN_INANITY_DATA

ptr<File> TempFileSystem::TryLoadFile(const String& fileName)
{
	Files::const_iterator i = files.find(fileName);
	return (i == files.end()) ? nullptr : i->second;
}

void TempFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	files[fileName] = file;
}

void TempFileSystem::GetFileNames(std::vector<String>& fileNames) const
{
	for(Files::const_iterator i = files.begin(); i != files.end(); ++i)
		fileNames.push_back(i->first);
}

END_INANITY_DATA
