#include "BufferedInputStream.hpp"
#include "../MemoryFile.hpp"
#include <algorithm>
#include <cstring>

BEGIN_INANITY_DATA

BufferedInputStream::BufferedInputStream(ptr<InputStream> stream, size_t bufferSize)
	: stream(stream), bufferSize(bufferSize), dataBegin(0), dataEnd(0), sourceExhausted(false)
{
	bufferFile = NEW(MemoryFile(bufferSize));
}

size_t BufferedInputStream::ReadSource(void* data, size_t size)
{
	if(sourceExhausted)
		return 0;
	size_t read = stream->Read(data, size);
	if(read < size)
		sourceExhausted = true;
	return read;
}

bigsize_t BufferedInputStream::SkipSource(bigsize_t size)
{
	if(sourceExhausted)
		return 0;
	bigsize_t skipped = stream->Skip(size);
	if(skipped < size)
		sourceExhausted = true;
	return skipped;
}

size_t BufferedInputStream::Read(void* data, size_t size)
{
	char* dataPtr = (char*)data;
	char* bufferPtr = (char*)bufferFile->GetData();

	// number of read bytes
	size_t read = 0;

	// copy all that we have in buffer
	size_t copySize = std::min(size, dataEnd - dataBegin);
	memcpy(dataPtr, bufferPtr + dataBegin, copySize);
	dataBegin += copySize;
	dataPtr += copySize;
	size -= copySize;
	read += copySize;

	// if no more data is needed
	if(!size)
		return read;

	// else buffer was exhausted
	// if consumer needs more data than buffer size, just read them
	if(size >= bufferSize)
		read += ReadSource(dataPtr, size);
	// else read data into buffer
	// at this point buffer is always empty
	else
	{
		dataBegin = 0;
		dataEnd = ReadSource(bufferPtr, bufferSize);
		copySize = std::min(size, dataEnd);
		memcpy(dataPtr, bufferPtr, copySize);
		dataBegin += copySize;
		read += copySize;
	}

	return read;
}

bigsize_t BufferedInputStream::Skip(bigsize_t size)
{
	// if we need to skip less data than in the buffer
	if(size <= (bigsize_t)(dataEnd - dataBegin))
	{
		// just move the pointer
		dataBegin += (size_t)size;
		return size;
	}

	// number of skipped bytes
	bigsize_t skipped = 0;

	// else we need more data than currenty in buffer
	// discard buffer completely
	skipped += (dataEnd - dataBegin);
	size -= (dataEnd - dataBegin);
	dataBegin = dataEnd;

	// if we have to skip more data than buffer size, just skip it
	if(size >= bufferSize)
		skipped += SkipSource(size);
	// else we have to skip small amount, so read into the buffer
	// at this point buffer is always empty
	else
	{
		char* bufferPtr = (char*)bufferFile->GetData();
		dataEnd = ReadSource(bufferPtr, bufferSize);
		dataBegin = std::min((size_t)size, dataEnd);
		skipped += dataBegin;
	}

	return skipped;
}

END_INANITY_DATA
