#ifndef ___INANITY_DATA_CONVERTING_FILE_SYSTEM_HPP___
#define ___INANITY_DATA_CONVERTING_FILE_SYSTEM_HPP___

#include "data.hpp"
#include "../FileSystem.hpp"
#include <functional>

BEGIN_INANITY_DATA

/// FileSystem applying convertor on-the-fly to data from underlying filesystem.
class ConvertingFileSystem : public FileSystem
{
private:
	ptr<FileSystem> fileSystem;
	typedef std::function<ptr<InputStream>(ptr<InputStream>)> InputConvertor;
	InputConvertor inputConvertor;

public:
	ConvertingFileSystem(ptr<FileSystem> fileSystem, InputConvertor&& inputConvertor);

	// FileSystem's methods.
	ptr<File> LoadFile(const String& fileName) override;
	ptr<File> TryLoadFile(const String& fileName) override;
	ptr<InputStream> LoadStream(const String& fileName) override;
};

END_INANITY_DATA

#endif
