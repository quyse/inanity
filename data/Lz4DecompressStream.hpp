#ifndef ___INANITY_DATA_LZ4_DECOMPRESS_STREAM_HPP___
#define ___INANITY_DATA_LZ4_DECOMPRESS_STREAM_HPP___

#include "data.hpp"
#include "../StreamReader.hpp"

BEGIN_INANITY

class MemoryFile;

END_INANITY

BEGIN_INANITY_DATA

/// Stream to decompress data compressed by LZ4 algorithm.
class Lz4DecompressStream : public InputStream
{
private:
	static const size_t inputBufferSize;
	static const size_t outputBufferSize;
	/// Prefix contains previous block.
	static const size_t outputBufferPrefix;

	StreamReader reader;
	ptr<MemoryFile> inputFile;
	ptr<MemoryFile> outputFile;
	size_t outputBegin, outputEnd;
	bool sourceExhausted;

	size_t ReadSource(void* data);

public:
	Lz4DecompressStream(ptr<InputStream> inputStream);

	size_t Read(void* data, size_t size);

	static ptr<InputStream> ConvertStream(ptr<InputStream> inputStream);
};

END_INANITY_DATA

#endif
