#include "FileInputStream.hpp"
#include "File.hpp"
#include <memory.h>

FileInputStream::FileInputStream(ptr<File> file) : file(file), fileData((char*)file->GetData()), fileSize(file->GetSize())
{
}

size_t FileInputStream::Read(void *data, size_t size)
{
	if(size > fileSize)
		size = fileSize;
	memcpy(data, fileData, size);
	fileData += size;
	fileSize -= size;
	return size;
}
