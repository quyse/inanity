#ifndef ___INANITY_BUFFERED_INPUT_STREAM_HPP___
#define ___INANITY_BUFFERED_INPUT_STREAM_HPP___

#include "InputStream.hpp"

BEGIN_INANITY

class File;

/// Класс входного потока, буферизующий ввод.
/** Предназначен для снижения нагрузки при чтении из потоков, которые
плохо относятся к множественным операциям. */
class BufferedInputStream : public InputStream
{
public:
	static const size_t defaultBufferSize = 0x10000;

private:
	ptr<InputStream> stream;
	ptr<File> bufferFile;
	size_t bufferSize, dataBegin, dataEnd;

public:
	/// Создать буферизированный поток с заданным размером буфера.
	BufferedInputStream(ptr<InputStream> stream, size_t bufferSize = defaultBufferSize);

	size_t Read(void* data, size_t size);
};

END_INANITY

#endif
