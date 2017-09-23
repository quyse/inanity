#include "Source.hpp"
#include "../MemoryStream.hpp"
#include "../FileInputStream.hpp"
#include "../File.hpp"

BEGIN_INANITY_AUDIO

size_t Source::GetSize() const
{
	Format format = GetFormat();
	return GetSamplesCount() * format.bitsPerSample * format.channelsCount / 8;
}

ptr<File> Source::GetData()
{
	ptr<MemoryStream> stream = NEW(MemoryStream(GetSize()));
	stream->ReadAllFromStream(CreateStream());
	return stream->ToFile();
}

ptr<InputStream> Source::CreateStream()
{
	return NEW(FileInputStream(GetData()));
}

END_INANITY_AUDIO
