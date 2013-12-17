#ifndef ___INANITY_DATA_DEFLATE_STREAM_HPP___
#define ___INANITY_DATA_DEFLATE_STREAM_HPP___

#include "data.hpp"
#include "../OutputStream.hpp"
#include "../meta/decl.hpp"
#include "../zlib.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_DATA

/// Stream to compress data with DEFLATE method.
class DeflateStream : public OutputStream
{
public:
	enum CompressionLevel
	{
		compressionNo = 0,
		compressionMin = 1,
		compressionMax = 9,
		compressionDefault = -1
	};

private:
	static const unsigned inputBufferSize = 0x10000;

	ptr<File> inputFile;
	ptr<File> outputFile;
	z_stream zstream;
	ptr<OutputStream> outputStream;

	bool finalized;

	void WriteOutput(void* data, size_t size);

public:
	DeflateStream(ptr<OutputStream> outputStream, CompressionLevel compressionLevel);
	~DeflateStream();

	static ptr<File> CompressFile(ptr<File> file, CompressionLevel compressionLevel);

	void Write(const void* data, size_t size);

	void Flush();

	static ptr<DeflateStream> CreateMax(ptr<OutputStream> outputStream);

	META_DECLARE_CLASS(DeflateStream);
};

END_INANITY_DATA

#endif
