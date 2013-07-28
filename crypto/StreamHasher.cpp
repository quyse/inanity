#include "StreamHasher.hpp"
#include "HashAlgorithm.hpp"
#include "HashStream.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"
#include <cstring>

BEGIN_INANITY_CRYPTO

StreamHasher::StreamHasher(ptr<HashAlgorithm> hashAlgorithm, size_t blockSize)
: hashAlgorithm(hashAlgorithm), blockSize(blockSize)
{
}

ptr<StreamHasher::HasherStream> StreamHasher::CreateHasherStream(ptr<OutputStream> destStream)
{
	return NEW(HasherStream(this, destStream));
}

ptr<StreamHasher::VerifyStream> StreamHasher::CreateVerifyStream(ptr<InputStream> sourceDataStream, ptr<InputStream> sourceHashStream)
{
	return NEW(VerifyStream(this, sourceDataStream, sourceHashStream));
}

StreamHasher::HasherStream::HasherStream(ptr<StreamHasher> hasher, ptr<OutputStream> destStream)
: hashStream(hasher->hashAlgorithm->CreateHashStream()), hashSize(hashStream->GetHashSize()),
blockSize(hasher->blockSize), destStream(destStream), blockDataSize(0)
{
	blockFile = NEW(MemoryFile(blockSize));
	block = (char*)blockFile->GetData();
}

void StreamHasher::HasherStream::WriteBlock()
{
	hashStream->Reset();
	hashStream->Write(block, blockDataSize);
	hashStream->End();
	void* hash = alloca(hashSize);
	hashStream->GetHash(hash);
	destStream->Write(hash, hashSize);

	blockDataSize = 0;
}

void StreamHasher::HasherStream::Write(const void* data, size_t size)
{
	const char* dataPtr = (const char*)data;
	while(size)
	{
		size_t toWrite = blockSize - blockDataSize;
		if(toWrite > size)
			toWrite = size;
		memcpy(block + blockDataSize, dataPtr, toWrite);
		blockDataSize += toWrite;
		size -= toWrite;
		dataPtr += toWrite;

		if(blockDataSize >= blockSize)
			WriteBlock();
	}
}

void StreamHasher::HasherStream::Flush()
{
	if(blockDataSize)
		WriteBlock();
}

StreamHasher::VerifyStream::VerifyStream(ptr<StreamHasher> streamHasher, ptr<InputStream> sourceDataStream, ptr<InputStream> sourceHashStream)
: hashStream(streamHasher->hashAlgorithm->CreateHashStream()), hashSize(hashStream->GetHashSize()),
blockSize(streamHasher->blockSize), sourceDataStream(sourceDataStream), sourceHashStream(sourceHashStream),
blockReadSize(0), blockDataSize(0)
{
	blockFile = NEW(MemoryFile(blockSize));
	block = (char*)blockFile->GetData();
}

void StreamHasher::VerifyStream::ReadBlock()
{
	try
	{
		// считать исходные данные
		blockDataSize = sourceDataStream->Read(block, blockSize);
		blockReadSize = 0;
		// считать оригинальный хеш (тот, который должен быть)
		void* originalHash = alloca(hashSize);
		size_t originalHashSize = sourceHashStream->Read(originalHash, hashSize);
		// если исходные данные есть, то и хеш должен быть
		// а если исходных данных нет, то и хеша не должно
		if(blockDataSize)
		{
			if(originalHashSize != hashSize)
				THROW("Can't read hash");
		}
		else
		{
			if(originalHashSize)
				THROW("Extra hash after data end");
			// данных нет, и больше делать нечего
			return;
		}

		// получить хеш исходных данных
		hashStream->Reset();
		hashStream->Write(block, blockDataSize);
		hashStream->End();
		void* dataHash = alloca(hashSize);
		hashStream->GetHash(dataHash);

		// сравнить его с оригинальным
		if(memcmp(originalHash, dataHash, hashSize) != 0)
			THROW("Wrong data hash");

		// всё хорошо, данные можно использовать
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't read block in verify stream of stream hasher", exception);
	}
}

size_t StreamHasher::VerifyStream::Read(void* data, size_t size)
{
	char* dataPtr = (char*)data;
	while(size)
	{
		if(blockReadSize >= blockDataSize)
			ReadBlock();
		size_t toRead = blockDataSize - blockReadSize;
		if(!toRead)
			break;
		if(toRead > size)
			toRead = size;
		memcpy(dataPtr, block + blockReadSize, toRead);
		blockReadSize += toRead;
		size -= toRead;
		dataPtr += toRead;
	}
	return dataPtr - (char*)data;
}

END_INANITY_CRYPTO
