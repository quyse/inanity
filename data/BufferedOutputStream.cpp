#include "BufferedOutputStream.hpp"
#include "../MemoryFile.hpp"
#include <algorithm>
#include <cstring>

BEGIN_INANITY_DATA

BufferedOutputStream::BufferedOutputStream(ptr<OutputStream> stream, size_t bufferSize)
	: stream(stream), bufferSize(bufferSize), dataSize(0)
{
	bufferFile = NEW(MemoryFile(bufferSize));
}

BufferedOutputStream::~BufferedOutputStream()
{
	FlushBuffer();
}

void BufferedOutputStream::Write(const void* data, size_t size)
{
	unsigned char* dataPtr = (unsigned char*)data;
	unsigned char* bufferPtr = (unsigned char*)bufferFile->GetData();

	//сначала дописать в буфер
	size_t copySize = std::min(size, bufferSize - dataSize);
	memcpy(bufferPtr + dataSize, dataPtr, copySize);
	dataSize += copySize;
	dataPtr += copySize;
	size -= copySize;

	//если данных больше не требуется
	if(!size)
		return;

	//иначе сначала очистить буфер
	FlushBuffer();

	//если данных дали не меньше, чем размер буфера, то просто записать их в поток
	if(size >= bufferSize)
		stream->Write(dataPtr, size);
	//иначе нужно заполнить буфер из потока
	//(если управление здесь, то буфер пуст)
	else
	{
		memcpy(bufferPtr, dataPtr, size);
		dataSize = size;
		//в изменении dataPtr и size больше нет надобности
	}
}

void BufferedOutputStream::FlushBuffer()
{
	if(dataSize)
	{
		stream->Write(bufferFile->GetData(), dataSize);
		dataSize = 0;
	}
}

void BufferedOutputStream::Flush()
{
	FlushBuffer();
}

END_INANITY_DATA
