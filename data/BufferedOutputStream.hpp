#ifndef ___INANITY_DATA_BUFFERED_OUTPUT_STREAM_HPP___
#define ___INANITY_DATA_BUFFERED_OUTPUT_STREAM_HPP___

#include "data.hpp"
#include "../OutputStream.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_DATA

/// Класс выходного потока, буферизующий вывод.
/** Предназначен для снижения нагрузки при записи в потоки, которые
плохо относятся к множественным операциям. */
class BufferedOutputStream : public OutputStream
{
public:
	static const size_t defaultBufferSize = 0x10000;

private:
	ptr<OutputStream> stream;
	ptr<File> bufferFile;
	size_t bufferSize, dataSize;

	void FlushBuffer();

public:
	/// Создать буферизированный поток с заданным размером буфера.
	BufferedOutputStream(ptr<OutputStream> stream, size_t bufferSize = defaultBufferSize);
	~BufferedOutputStream();

	void Write(const void* data, size_t size);

	void Flush();
};

END_INANITY_DATA

#endif
