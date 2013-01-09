#include "OggVorbisSource.hpp"
#include "OggVorbisStream.hpp"
#include "../File.hpp"

OggVorbisSource::OggVorbisSource(ptr<File> file) : file(file)
{
	// получить формат файла
	OggVorbisStream::InternalGetFormat(ptr<File> file);
}

Format OggVorbisSource::GetFormat() const
{
	return format;
}

ptr<InputStream> OggVorbisSource::CreateStream()
{
	return NEW(OggVorbisStream(file));
}
