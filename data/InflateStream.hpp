#ifndef ___INANITY_DATA_INFLATE_STREAM_HPP___
#define ___INANITY_DATA_INFLATE_STREAM_HPP___

#include "data.hpp"
#include "../InputStream.hpp"
#include "../meta/decl.hpp"
#include "../zlib.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_DATA

/// Stream to decompress DEFLATE-compressed data.
class InflateStream : public InputStream
{
private:
	static const unsigned inputBufferSize = 0x10000;
	static const unsigned outputBufferSize = 0x80000;

	ptr<File> inputFile;
	ptr<File> outputFile;
	z_stream zstream;
	Bytef* outputData;
	ptr<InputStream> inputStream;
	bool finished;

public:
	InflateStream(ptr<InputStream> inputStream);
	~InflateStream();

	static ptr<File> DecompressFile(ptr<File> file);

	size_t Read(void* data, size_t size);

	META_DECLARE_CLASS(InflateStream);
};

END_INANITY_DATA

#endif
