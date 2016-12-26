#include "Lz4DecompressStream.hpp"
#include "Lz4CompressStream.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"
#include "../deps/lz4/lz4.h"
#include <memory.h>
#include <algorithm>

BEGIN_INANITY_DATA

const size_t Lz4DecompressStream::outputBufferPrefix = 0x10000;
const size_t Lz4DecompressStream::outputBufferSize =
	Lz4DecompressStream::outputBufferPrefix +
	Lz4CompressStream::inputBufferSize;
const size_t Lz4DecompressStream::inputBufferSize =
	LZ4_COMPRESSBOUND(Lz4DecompressStream::outputBufferSize);

Lz4DecompressStream::Lz4DecompressStream(ptr<InputStream> inputStream)
: reader(inputStream), outputBegin(0), outputEnd(0), sourceExhausted(false)
{
	inputFile = NEW(MemoryFile(inputBufferSize));
	outputFile = NEW(MemoryFile(outputBufferSize));
	// fill prefix with zeroes
	memset(outputFile->GetData(), 0, outputBufferPrefix);
}

size_t Lz4DecompressStream::ReadSource(void* data)
{
	if(sourceExhausted)
		return 0;

	// read block size
	size_t size;
	try
	{
		size = reader.ReadShortly();
	}
	catch(Exception* exception)
	{
		// process end of file
		MakePointer(exception);
		sourceExhausted = true;
		return 0;
	}

	if(size > inputBufferSize)
		THROW("Too big block");

	reader.Read(data, size);
	return size;
}

size_t Lz4DecompressStream::Read(void* data, size_t size)
{
	char* dataPtr = (char*)data;
	char* inputData = (char*)inputFile->GetData();
	char* outputData = (char*)outputFile->GetData();

	size_t read = 0;

	while(size)
	{
		// if output buffer is empty, get more data
		if(outputBegin >= outputEnd)
		{
			// read input data
			size_t inputSize = ReadSource(inputData);
			// if there is no more data, that's end
			if(!inputSize)
				break;

			// move last 64 kb of output to buffer's start, if they present
			size_t prefixSize = std::min(outputEnd, outputBufferPrefix);
			memmove(outputData, outputData + outputEnd - prefixSize, prefixSize);

			// decompress data
			outputBegin = outputBufferPrefix;
			size_t outputSize = LZ4_decompress_safe_usingDict(inputData, outputData + outputBegin, inputSize, outputBufferSize - outputBegin, outputData, prefixSize);
			if(!outputSize)
				THROW("Can't decompress LZ4 data");
			outputEnd = outputBegin + outputSize;
		}

		// read data from output buffer
		size_t copySize = std::min(size, outputEnd - outputBegin);
		memcpy(dataPtr, outputData + outputBegin, copySize);
		outputBegin += copySize;
		dataPtr += copySize;
		size -= copySize;
		read += copySize;
	}

	return read;
}

END_INANITY_DATA
