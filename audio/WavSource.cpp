#include "WavSource.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

struct WavSource::Header
{
	uint32_t riffMagic; // "RIFF"
	uint32_t chunkSize; // size of file - 8
	uint32_t waveMagic; // "WAVE"
	uint32_t fmtMagic;  // "fmt "
	uint32_t fmtSize;   // 16
	uint16_t audioFormat; // 1 for PCM
	uint16_t numChannels; // 1 for mono, 2 for stereo, ...
	uint32_t sampleRate; // e.g. 44100
	uint32_t byteRate; // sampleRate * numChannels * bitsPerSample / 8
	uint16_t blockAlign; // numChannels * bitsPerSample / 8
	uint16_t bitsPerSample; // 8, 16, ...
	uint32_t dataMagic; // "data"
	uint32_t dataSize;  // data size = numSamples * numChannels * bitsPerSample / 8
};

WavSource::WavSource(ptr<File> file)
{
	BEGIN_TRY();

	// check file size
	size_t size = file->GetSize();
	if(size < sizeof(Header))
		THROW("File is too small");

	const Header* header = (const Header*)file->GetData();
	// sanity check
	if(header->riffMagic != 'FFIR') THROW("Wrong RIFF magic");
	if(header->waveMagic != 'EVAW') THROW("Wrong WAVE magic");
	if(header->fmtMagic != ' tmf') THROW("Wrong fmt magic");
	if(header->audioFormat != 1) THROW("Wrong audio format");
	if(header->dataMagic != 'atad') THROW("Wrong data magic");
	if(header->dataSize + 40 > size) THROW("Wrong data size");

	// prepare data
	dataFile = file->Slice(40, header->dataSize);
	format.samplesPerSecond = header->sampleRate;
	format.bitsPerSample = (uint8_t)header->bitsPerSample;
	format.channelsCount = (uint8_t)header->numChannels;
	samplesCount = header->dataSize / (format.channelsCount * format.bitsPerSample / 8);

	END_TRY("Can't load WAV source");
}

Format WavSource::GetFormat() const
{
	return format;
}

size_t WavSource::GetSamplesCount() const
{
	return samplesCount;
}

ptr<File> WavSource::GetData()
{
	return dataFile;
}

END_INANITY_AUDIO
