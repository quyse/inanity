#ifndef ___INANITY_DATA_LZ4_COMPRESS_STREAM_HPP___
#define ___INANITY_DATA_LZ4_COMPRESS_STREAM_HPP___

#include "data.hpp"
#include "../StreamWriter.hpp"

BEGIN_INANITY

class MemoryFile;

END_INANITY

BEGIN_INANITY_DATA

class Lz4DecompressStream;

/// Stream to compress data with LZ4 algorithm.
class Lz4CompressStream : public OutputStream
{
	friend class Lz4DecompressStream;
private:
	static const size_t inputBufferSize;

	StreamWriter writer;
	ptr<MemoryFile> inputFile;
	ptr<MemoryFile> outputFile;
	size_t inputBegin, inputEnd;
	/// Internal LZ4 structure.
	void* internal;

public:
	Lz4CompressStream(ptr<OutputStream> outputStream);
	~Lz4CompressStream();

	//*** OutputStream's methods.
	void Write(const void* data, size_t size);

	/// Flush delayed data into output stream.
	/** It's allowed to call this multiple times. */
	void Flush();
};

END_INANITY_DATA

#endif
