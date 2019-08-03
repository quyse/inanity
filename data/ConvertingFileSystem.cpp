#include "ConvertingFileSystem.hpp"
#include "../FileInputStream.hpp"
#include "../MemoryStream.hpp"
#include "../File.hpp"

BEGIN_INANITY_DATA

ConvertingFileSystem::ConvertingFileSystem(ptr<FileSystem> fileSystem, InputConvertor&& inputConvertor)
: fileSystem(std::move(fileSystem)), inputConvertor(inputConvertor) {}

ptr<File> ConvertingFileSystem::LoadFile(const String& fileName)
{
	ptr<InputStream> inputStream = inputConvertor(NEW(FileInputStream(fileSystem->LoadFile(fileName))));
	ptr<MemoryStream> outputStream = NEW(MemoryStream);
	outputStream->ReadAllFromStream(inputStream);
	return outputStream->ToFile();
}

ptr<File> ConvertingFileSystem::TryLoadFile(const String& fileName)
{
	ptr<File> file = fileSystem->TryLoadFile(fileName);
	if(!file) return nullptr;
	ptr<InputStream> inputStream = inputConvertor(NEW(FileInputStream(file)));
	ptr<MemoryStream> outputStream = NEW(MemoryStream);
	outputStream->ReadAllFromStream(inputStream);
	return outputStream->ToFile();
}

ptr<InputStream> ConvertingFileSystem::LoadStream(const String& fileName)
{
	return inputConvertor(fileSystem->LoadStream(fileName));
}

END_INANITY_DATA
