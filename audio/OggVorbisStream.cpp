#include "OggVorbisStream.hpp"
#include "../Storage.hpp"
#include "../Exception.hpp"
#include <memory.h>

BEGIN_INANITY_AUDIO

const ov_callbacks OggVorbisStream::oggVorbisCallbacks =
{
	OggVorbisStream::ReadFunctionStatic,
	OggVorbisStream::SeekFunctionStatic,
	0,
	OggVorbisStream::TellFunctionStatic
};

size_t OggVorbisStream::ReadFunctionStatic(void* data, size_t size, size_t count, void* stream)
{
	return ((OggVorbisStream*)stream)->ReadFunction(data, size, count);
}

size_t OggVorbisStream::ReadFunction(void* data, size_t size, size_t count)
{
	// determine how many chunks can be read
	bigsize_t toEnd = storage->GetBigSize() - position;
	bigsize_t maxCount = toEnd / size;
	// saturate
	if(count > maxCount)
		count = (size_t)maxCount;
	// read data
	if(count)
	{
		size *= count;
		storage->Read(position, size, data);
		position += size;
	}
	return count;
}

int OggVorbisStream::SeekFunctionStatic(void* stream, ogg_int64_t offset, int whence)
{
	return ((OggVorbisStream*)stream)->SeekFunction((long long)offset, whence);
}

int OggVorbisStream::SeekFunction(long long offset, int whence)
{
	long long newPosition;
	bigsize_t fileSize = storage->GetBigSize();
	switch(whence)
	{
	case SEEK_SET:
		newPosition = offset;
		break;
	case SEEK_CUR:
		newPosition = position + offset;
		break;
	case SEEK_END:
		newPosition = offset + fileSize;
		break;
	default:
		return -1;
	}
	if(newPosition < 0 || (bigsize_t)newPosition > fileSize) return -1;
	position = (bigsize_t)newPosition;
	return 0;
}

long OggVorbisStream::TellFunctionStatic(void* stream)
{
	return ((OggVorbisStream*)stream)->TellFunction();
}

long OggVorbisStream::TellFunction()
{
	return (long)position;
}

OggVorbisStream::OggVorbisStream(ptr<Storage> storage)
: storage(storage), position(0)
{
	BEGIN_TRY();

	// open file
	int r = ov_open_callbacks(this, &ovFile, 0, -1, oggVorbisCallbacks);
	if(r != 0)
		THROW("Can't open file as Ogg Vorbis");

	// read file info
	vorbis_info* info = ov_info(&ovFile, -1);
	if(!info)
		THROW("Can't get information about file");

	// remember format
	format.samplesPerSecond = info->rate;
	format.bitsPerSample = 16;
	format.channelsCount = info->channels;

	// get samples count
	samplesCount = (size_t)ov_pcm_total(&ovFile, -1);

	END_TRY("Can't create Ogg Vorbis stream");
}

OggVorbisStream::~OggVorbisStream()
{
	ov_clear(&ovFile);
}

Format OggVorbisStream::GetFormat() const
{
	return format;
}

size_t OggVorbisStream::GetSamplesCount() const
{
	return samplesCount;
}

size_t OggVorbisStream::Read(void* dataPtr, size_t size)
{
	BEGIN_TRY();

	char* data = (char*)dataPtr;

	size_t read = 0;

	int bitstream;
	while(size)
	{
		long result = ov_read(&ovFile, data, (int)size,
			0, // means little endian
			2, // means 16-bit samples
			1, // means signed values
			&bitstream);
		// if this is an error
		if(result < 0)
			THROW("Ogg Vorbis file is corrupted");
		// if end of file, break
		if(result == 0)
			break;

		size -= result;
		data += result;
		read += result;
	}

	return read;

	END_TRY("Can't read data from Ogg Vorbis file");
}

END_INANITY_AUDIO
