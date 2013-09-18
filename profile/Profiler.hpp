#ifndef ___INANITY_PROFILE_PROFILER_HPP___
#define ___INANITY_PROFILE_PROFILER_HPP___

#include "RecordType.hpp"

#ifdef ___INANITY_PROFILING

#include "../CriticalSection.hpp"
#include <vector>
#include <iostream>

BEGIN_INANITY_PROFILE

/// Main profiler class.
class Profiler
{
private:
	/// Number of records in one chunk.
	static const size_t chunkRecordsCount;

public:
	struct Record
	{
		long long time;
		RecordType type;
		const char* note;
	};

	struct Chunk
	{
		Record* recordsBegin;
		Record* recordsEnd;
		Chunk* nextChunk;

		Chunk();
	};

	class ChunkIterator
	{
	private:
		Chunk* chunk;

	public:
		ChunkIterator(Chunk* chunk);

		operator bool() const;
		Chunk* operator*() const;
		ChunkIterator operator++(int);
		ChunkIterator& operator++();
	};

	class RecordIterator
	{
	private:
		ChunkIterator chunkIterator;
		Record* currentRecord;

	public:
		RecordIterator(ChunkIterator chunkIterator);
		RecordIterator(ChunkIterator chunkIterator, Record* currentRecord);

		operator bool() const;
		Record* operator*() const;
		RecordIterator operator++(int);
		RecordIterator& operator++();
	};

	class ThreadChunkIterator
	{
	private:
		const Profiler& profiler;
		size_t index;

	public:
		ThreadChunkIterator(const Profiler& profiler, size_t index);

		operator bool() const;
		ChunkIterator operator*() const;
		ThreadChunkIterator operator++(int);
		ThreadChunkIterator& operator++();
	};

private:
	CriticalSection cs;
	std::vector<Chunk*> threadChunks;

public:
	Profiler();
	~Profiler();

	void FlushChunk(Chunk*& chunk, Record*& records, Record*& recordsEnd);

	/// Get thread chunk iterator.
	ThreadChunkIterator GetThreadChunkIterator() const;
};

END_INANITY_PROFILE

#endif

#endif
