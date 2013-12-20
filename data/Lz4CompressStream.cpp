#include "Lz4CompressStream.hpp"
#include "../MemoryFile.hpp"
#include "../deps/lz4/lz4.h"
#include <algorithm>

BEGIN_INANITY_DATA

const size_t Lz4CompressStream::inputBufferSize = 0x40000;

Lz4CompressStream::Lz4CompressStream(ptr<OutputStream> outputStream)
: writer(outputStream), inputBegin(0), inputEnd(0)
{
	inputFile = NEW(MemoryFile(inputBufferSize));
	outputFile = NEW(MemoryFile(LZ4_compressBound(inputBufferSize)));
	internal = LZ4_create((const char*)inputFile->GetData());
}

Lz4CompressStream::~Lz4CompressStream()
{
	LZ4_free(internal);
}

void Lz4CompressStream::Write(const void* data, size_t size)
{
	const char* dataPtr = (const char*)data;
	char* inputData = (char*)inputFile->GetData();
	char* outputData = (char*)outputFile->GetData();

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
		size_t outputSize = LZ4_compress_continue(internal, inputData + inputBegin, outputData, inputEnd - inputBegin);
		writer.WriteShortly(outputSize);
		writer.Write(outputData, outputSize);
		inputBegin = LZ4_slideInputBuffer(internal) - inputData;
		inputEnd = inputBegin;
	}
}

END_INANITY_DATA
