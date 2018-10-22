#include "Lz4CompressStream.hpp"
#include "../MemoryFile.hpp"
#include "../deps/lz4/lz4.h"
#include <memory.h>
#include <algorithm>

BEGIN_INANITY_DATA

const size_t Lz4CompressStream::inputBufferSize = 0x40000;
const size_t Lz4CompressStream::dictSize = 0x10000;
const size_t Lz4CompressStream::outputBufferSize = LZ4_COMPRESSBOUND(Lz4CompressStream::inputBufferSize);

Lz4CompressStream::Lz4CompressStream(ptr<OutputStream> outputStream)
: writer(outputStream), inputBegin(0), inputEnd(0)
{
	inputFile = NEW(MemoryFile(inputBufferSize));
	dictFile = NEW(MemoryFile(dictSize));
	outputFile = NEW(MemoryFile(outputBufferSize));
	internal = LZ4_createStream();
}

Lz4CompressStream::~Lz4CompressStream()
{
	LZ4_freeStream((LZ4_stream_t*)internal);
}

void Lz4CompressStream::Write(const void* data, size_t size)
{
	const char* dataPtr = (const char*)data;
	char* inputData = (char*)inputFile->GetData();

	while(size)
	{
		// copy data into input buffer
		size_t copySize = std::min(size, inputBufferSize - inputEnd);
		memcpy(inputData + inputEnd, dataPtr, copySize);
		inputEnd += copySize;
		dataPtr += copySize;
		size -= copySize;

		// if input buffer is full, flush it
		if(inputEnd >= inputBufferSize)
			Flush();
	}
}

void Lz4CompressStream::Flush()
{
	if(inputBegin < inputEnd)
	{
		char* inputData = (char*)inputFile->GetData();
		char* outputData = (char*)outputFile->GetData();
		size_t outputSize = LZ4_compress_fast_continue((LZ4_stream_t*)internal, inputData + inputBegin, outputData, (int)(inputEnd - inputBegin), outputBufferSize, 1);
		writer.WriteShortly(outputSize);
		writer.Write(outputData, outputSize);
		LZ4_saveDict((LZ4_stream_t*)internal, (char*)dictFile->GetData(), dictSize);
		inputBegin = 0;
		inputEnd = 0;
	}
}

END_INANITY_DATA
