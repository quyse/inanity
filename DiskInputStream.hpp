#ifndef ___INANITY_DISK_INPUT_STREAM_HPP___
#define ___INANITY_DISK_INPUT_STREAM_HPP___

#include "InputStream.hpp"

/*
Файл содержит класс для потокового считывания из дискового файла.
*/

BEGIN_INANITY

class Handle;

class DiskInputStream : public InputStream
{
private:
	ptr<Handle> handle;

public:
	DiskInputStream(ptr<Handle> handle);

	size_t Read(void* data, size_t size);
	size_t GetSize() const;
};

END_INANITY

#endif
