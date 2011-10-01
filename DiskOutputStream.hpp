#ifndef ___INANITY_DISK_OUTPUT_STREAM_HPP___
#define ___INANITY_DISK_OUTPUT_STREAM_HPP___

#include "OutputStream.hpp"

/*
Файл содержит класс для потоковой записи в дисковый файл.
*/

BEGIN_INANITY

class Handle;

class DiskOutputStream : public OutputStream
{
private:
	ptr<Handle> handle;

public:
	DiskOutputStream(ptr<Handle> handle);

	void Write(const void* data, size_t size);
};

END_INANITY

#endif
