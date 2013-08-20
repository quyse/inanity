#include "OggVorbisSource.hpp"
#include "OggVorbisStream.hpp"
#include "../File.hpp"
#include "../MemoryStream.hpp"

BEGIN_INANITY_AUDIO

OggVorbisSource::OggVorbisSource(ptr<File> file) : file(file)
{
	// get info from temporary stream
	ptr<OggVorbisStream> stream = NEW(OggVorbisStream(file));
	format = stream->GetFormat();
	samplesCount = stream->GetSamplesCount();
}

Format OggVorbisSource::GetFormat() const
{
	return format;
}

size_t OggVorbisSource::GetSamplesCount() const
{
	return samplesCount;
}

ptr<File> OggVorbisSource::GetData()
{
	if(!dataFile)
	{
		ptr<MemoryStream> stream = NEW(MemoryStream());
		stream->ReadAllFromStream(CreateStream());
		dataFile = stream->ToFile();
	}

	return dataFile;
}

ptr<InputStream> OggVorbisSource::CreateStream()
{
	return NEW(OggVorbisStream(file));
}

END_INANITY_AUDIO
