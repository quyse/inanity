#include "Profiler.hpp"

#ifdef ___INANITY_PROFILING

#include "../Time.hpp"
#include <cstring>

BEGIN_INANITY_PROFILE

const size_t Profiler::chunkRecordsCount = 0x10000;

Profiler::Chunk::Chunk() : recordsBegin(0), recordsEnd(0), nextChunk(0) {}

Profiler::Profiler() {}

Profiler::~Profiler()
{
	for(size_t i = 0; i < threadChunks.size(); ++i)
	{
		Chunk* nextChunk;
		for(Chunk* chunk = threadChunks[i]; chunk; chunk = nextChunk)
		{
			nextChunk = chunk->nextChunk;
			delete [] chunk->recordsBegin;
			delete chunk;
		}
	}
	threadChunks.clear();
}

void Profiler::FlushChunk(Chunk*& chunk, Record*& records, Record*& recordsEnd)
{
	// get time before operations
	long long startTime = Inanity::Time::GetTicks();

	// allocate new chunk
	Chunk* newChunk = new Chunk;

	// link chunks into list
	if(chunk)
		chunk->nextChunk = newChunk;
	else
		threadChunks.push_back(newChunk);

	// allocate memory for new chunk
	newChunk->recordsBegin = new Record[chunkRecordsCount];
	memset(newChunk->recordsBegin, 0, chunkRecordsCount * sizeof(Record));
	newChunk->recordsEnd = newChunk->recordsBegin + chunkRecordsCount;

	// fill variables
	chunk = newChunk;
	records = newChunk->recordsBegin;
	recordsEnd = newChunk->recordsEnd;

	// get time after operations
	long long finishTime = Inanity::Time::GetTicks();

	// issue records to skip time spent
	records->time = startTime;
	records->type = recordTypeStop;
	records->note = "Stop for flushing chunks";
	records++;
	records->time = finishTime;
	records->type = recordTypeStart;
	records->note = "Resume after flushing chunks";
	records++;
}

Profiler::ThreadChunkIterator Profiler::GetThreadChunkIterator() const
{
	return ThreadChunkIterator(*this, 0);
}

//*** Profiler::ChunkIterator class

Profiler::ChunkIterator::ChunkIterator(Chunk* chunk)
: chunk(chunk) {}

Profiler::ChunkIterator::operator bool() const
{
	return !!chunk;
}

Profiler::Chunk* Profiler::ChunkIterator::operator*() const
{
	return chunk;
}

Profiler::ChunkIterator Profiler::ChunkIterator::operator++(int)
{
	Chunk* currentChunk = chunk;
	++*this;
	return ChunkIterator(currentChunk);
}

Profiler::ChunkIterator& Profiler::ChunkIterator::operator++()
{
	chunk = chunk->nextChunk;
	return *this;
}

//*** Profiler::RecordIterator class

Profiler::RecordIterator::RecordIterator(ChunkIterator chunkIterator) :
	chunkIterator(chunkIterator),
	currentRecord(chunkIterator ? (*chunkIterator)->recordsBegin : 0)
{}

Profiler::RecordIterator::RecordIterator(ChunkIterator chunkIterator, Record* currentRecord)
: chunkIterator(chunkIterator), currentRecord(currentRecord) {}

Profiler::RecordIterator::operator bool() const
{
	return !!currentRecord;
}

Profiler::Record* Profiler::RecordIterator::operator*() const
{
	return currentRecord;
}

Profiler::RecordIterator Profiler::RecordIterator::operator++(int)
{
	RecordIterator i = *this;
	++*this;
	return i;
}

Profiler::RecordIterator& Profiler::RecordIterator::operator++()
{
	++currentRecord;
	for(;;)
	{
		if(currentRecord >= (*chunkIterator)->recordsEnd)
		{
			if(++chunkIterator)
			{
				currentRecord = (*chunkIterator)->recordsBegin;
				continue;
			}
			else
			{
				currentRecord = 0;
				break;
			}
		}

		if(currentRecord->type == recordTypeEmpty)
		{
			++currentRecord;
			continue;
		}

		break;
	}

	return *this;
}

//*** Profiler::ThreadChunkIterator class

Profiler::ThreadChunkIterator::ThreadChunkIterator(const Profiler& profiler, size_t index)
: profiler(profiler), index(index) {}

Profiler::ThreadChunkIterator::operator bool() const
{
	return index < profiler.threadChunks.size();
}

Profiler::ChunkIterator Profiler::ThreadChunkIterator::operator*() const
{
	return ChunkIterator(profiler.threadChunks[index]);
}

Profiler::ThreadChunkIterator Profiler::ThreadChunkIterator::operator++(int)
{
	return ThreadChunkIterator(profiler, index++);
}

Profiler::ThreadChunkIterator& Profiler::ThreadChunkIterator::operator++()
{
	++index;
	return *this;
}

END_INANITY_PROFILE

#endif
