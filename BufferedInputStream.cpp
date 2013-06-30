#include "BufferedInputStream.hpp"
#include "MemoryFile.hpp"
#include <algorithm>
#include <cstring>

BEGIN_INANITY

BufferedInputStream::BufferedInputStream(ptr<InputStream> stream, size_t bufferSize)
	: stream(stream), bufferSize(bufferSize), dataBegin(0), dataEnd(0)
{
	bufferFile = NEW(MemoryFile(bufferSize));
}

size_t BufferedInputStream::Read(void* data, size_t size)
{
	unsigned char* dataPtr = (unsigned char*)data;
	unsigned char* bufferPtr = (unsigned char*)bufferFile->GetData();

	//счётчик считанного
	size_t read = 0;

	//сначала скопировать всё, что есть в буфере
	size_t copySize = std::min(size, dataEnd - dataBegin);
	memcpy(dataPtr, bufferPtr + dataBegin, copySize);
	dataBegin += copySize;
	dataPtr += copySize;
	size -= copySize;
	read += copySize;

	//если данных больше не требуется
	if(!size)
		return read;

	//теперь, если данных требуют не меньше, чем размер буфера, то просто считать их
	if(size >= bufferSize)
		read += stream->Read(dataPtr, size);
	//иначе нужно заполнить буфер из потока
	//(если управление здесь, то буфер пуст)
	else
	{
		dataBegin = 0;
		dataEnd = stream->Read(bufferPtr, bufferSize);
		copySize = std::min(size, dataEnd);
		memcpy(dataPtr, bufferPtr, copySize);
		dataBegin += copySize;
		read += copySize;
		//в изменении dataPtr и size больше нет надобности
	}
	return read;
}

END_INANITY
