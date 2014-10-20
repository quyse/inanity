#include "OggVorbisStream.hpp"
#include "../File.hpp"
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
	// вычислить, сколько осталось до конца файла
	size_t toEnd = sourceFile->GetSize() - position;
	// вычислить, сколько элементов по size мы можем считать
	size_t maxCount = toEnd / size;
	// если требуется больше, то фиг вам
	if(count > maxCount)
		count = maxCount;
	// считать данные
	if(count)
	{
		size *= count;
		memcpy(data, (char*)sourceFile->GetData() + position, size);
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
	size_t fileSize = sourceFile->GetSize();
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
	if(newPosition < 0 || newPosition > fileSize) return -1;
	position = (size_t)newPosition;
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

OggVorbisStream::OggVorbisStream(ptr<File> sourceFile)
: sourceFile(sourceFile), position(0)
{
	try
	{
		// открыть файл как vorbis
		int r = ov_open_callbacks(this, &ovFile, 0, -1, oggVorbisCallbacks);
		if(r != 0)
		{
			printf("Error: %d\n", r);
			THROW("Can't open file as Ogg Vorbis");
		}

		// получить информацию о файле
		vorbis_info* info = ov_info(&ovFile, -1);
		if(!info)
			THROW("Can't get information about file");

		// запомнить формат
		format.samplesPerSecond = info->rate;
		format.bitsPerSample = 16;
		format.channelsCount = info->channels;

		// получить количество семплов
		samplesCount = (size_t)ov_pcm_total(&ovFile, -1);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create Ogg Vorbis stream", exception);
	}
}

OggVorbisStream::~OggVorbisStream()
{
	// закрыть файл vorbis
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
	try
	{
		char* data = (char*)dataPtr;

		size_t read = 0;

		int bitstream;
		while(size)
		{
			long result = ov_read(&ovFile, data, size,
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
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't read data from Ogg Vorbis file", exception);
	}
}

END_INANITY_AUDIO
