#ifndef ___INANITY_DATA_BUFFERED_INPUT_STREAM_HPP___
#define ___INANITY_DATA_BUFFERED_INPUT_STREAM_HPP___

#include "data.hpp"
#include "../InputStream.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_DATA

/// Input stream buffers input data.
/** It's good to use that with streams which has
bad performance with multiple (small) reads. */
class BufferedInputStream : public InputStream
{
public:
	static const size_t defaultBufferSize = 0x10000;

private:
	ptr<InputStream> stream;
	ptr<File> bufferFile;
	size_t bufferSize, dataBegin, dataEnd;
	/// Is source stream exhausted.
	bool sourceExhausted;

	size_t ReadSource(void* data, size_t size);
	bigsize_t SkipSource(bigsize_t size);

public:
	BufferedInputStream(ptr<InputStream> stream, size_t bufferSize = defaultBufferSize);

	size_t Read(void* data, size_t size);
	bigsize_t Skip(bigsize_t size);
};

END_INANITY_DATA

#endif
