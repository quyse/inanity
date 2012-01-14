#ifndef ___INANITY_FOLDER_FILE_HPP___
#define ___INANITY_FOLDER_FILE_HPP___

#include "File.hpp"

BEGIN_INANITY

class FolderFile : public File
{
private:
	void* data;
	size_t size;

public:
	FolderFile(void* data, size_t size);
	~FolderFile();

	void* GetData() const;
	size_t GetSize() const;
};

END_INANITY

#endif
