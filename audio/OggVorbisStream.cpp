#include "OggVorbisStream.hpp"
#include "../Exception.hpp"

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
	//вычислить, сколько осталось до конца файла
	unsigned toEnd = file->GetSize() - position;
	//вычислить, сколько элементов по size мы можем считать
	unsigned maxCount = toEnd / size;
	//если требуется больше, то фиг вам
	if(count > maxCount)
		count = maxCount;
	//считать данные
	if(count)
	{
		size *= count;
		memcpy(data, (char*)file->GetData() + position, size);
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
	size_t fileSize = file->GetSize();
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
	position = newPosition;
	return 0;
}

OggVorbisStream::OggVorbisStream(ptr<File> sourceFile) : sourceFile(sourceFile)
{
	try
	{
		// открыть файл как vorbis
		if(ov_open_callbacks(this, &oggVorbisFile, 0, -1, oggVorbisCallbacks) != 0)
			THROW_PRIMARY_EXCEPTION("Can't open file as Ogg Vorbis");

		// получить информацию о файле
		vorbis_info* info = ov_info(&oggVorbisFile, -1);
		if(!info)
			THROW_PRIMARY_EXCEPTION("Can't get information about file");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create Ogg Vorbis stream", exception);
	}
}

OggVorbisStream::~OggVorbisStream()
{
	// закрыть файл vorbis
	ov_clear(&oggVorbisFile);
}

size_t OggVorbisStream::Read(void* dataPtr, size_t size)
{
	try
	{
		char* data = (char*)dataPtr;
		size_t allSize = size;

		//считывать кусками из Ogg
		int bitstream;
		while(size)
		{
			//считать данные
			long result = ov_read(&oggVorbisFile, data, size, 0, format.wBitsPerSample == 16 ? 2 : 1, format.wBitsPerSample == 16 ? 1 : 0, &bitstream);
			//обработать ошибки
			if(result < 0)
				THROW_PRIMARY_EXCEPTION("Ogg Vorbis file is corrupted");
			if(result == 0)
				THROW_PRIMARY_EXCEPTION("End of Ogg Vorbis file");

			//если управление здесь, считано result байт
			size -= result;
			data += result;
		}

		//сдвинуть указатель текущего положения
		currentPosition += allSize;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't read data from Ogg Vorbis file", exception);
	}
}
