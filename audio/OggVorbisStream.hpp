#ifndef ___INANITY_AUDIO_OGG_VORBIS_STREAM_HPP___
#define ___INANITY_AUDIO_OGG_VORBIS_STREAM_HPP___

#include "../InputStream.hpp"
#include "Format.hpp"
#include "../deps/libvorbis/include/vorbis/vorbisfile.h"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

/// Класс потока аудиоданных Ogg Vorbis.
class OggVorbisStream : public InputStream
{
private:
	/// Исходный файл.
	ptr<File> sourceFile;
	/// Структура библиотеки vorbisfile.
	OggVorbis_File ovFile;
	/// Формат потока.
	Format format;
	/// Длина потока в семплах.
	size_t samplesCount;
	/// Текущее положение в файле.
	size_t position;

	/// Структура с функциями обратного вызова.
	static const ov_callbacks oggVorbisCallbacks;
	// Функции для структуры ov_callbacks.
	static size_t ReadFunctionStatic(void* data, size_t size, size_t count, void* dataSource);
	size_t ReadFunction(void* data, size_t size, size_t count);
	static int SeekFunctionStatic(void* stream, ogg_int64_t offset, int whence);
	int SeekFunction(long long offset, int whence);
	static long TellFunctionStatic(void* stream);
	long TellFunction();

public:
	OggVorbisStream(ptr<File> sourceFile);
	~OggVorbisStream();

	Format GetFormat() const;
	size_t GetSamplesCount() const;

	size_t Read(void* data, size_t size);
};

END_INANITY_AUDIO

#endif
