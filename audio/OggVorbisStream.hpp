#ifndef ___INANITY_AUDIO_OGG_VORBIS_STREAM_HPP___
#define ___INANITY_AUDIO_OGG_VORBIS_STREAM_HPP___

#include "../InputStream.hpp"
#include "Format.hpp"
#include "../deps/libvorbis/include/vorbis/vorbisfile.h"

BEGIN_INANITY

class Storage;

END_INANITY

BEGIN_INANITY_AUDIO

/// Ogg Vorbis decoding stream.
class OggVorbisStream : public InputStream
{
private:
	/// Source input storage.
	ptr<Storage> storage;
	OggVorbis_File ovFile;
	Format format;
	size_t samplesCount;
	bigsize_t position;

	/// Callbacks for Ogg Vorbis library.
	static const ov_callbacks oggVorbisCallbacks;
	// Static callback functions.
	static size_t ReadFunctionStatic(void* data, size_t size, size_t count, void* dataSource);
	size_t ReadFunction(void* data, size_t size, size_t count);
	static int SeekFunctionStatic(void* stream, ogg_int64_t offset, int whence);
	int SeekFunction(long long offset, int whence);
	static long TellFunctionStatic(void* stream);
	long TellFunction();

public:
	OggVorbisStream(ptr<Storage> storage);
	~OggVorbisStream();

	Format GetFormat() const;
	size_t GetSamplesCount() const;

	size_t Read(void* data, size_t size);
};

END_INANITY_AUDIO

#endif
