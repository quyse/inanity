#include "LamportSignatureAlgorithm.hpp"
#include "HashAlgorithm.hpp"
#include "HashStream.hpp"
#include "RandomAlgorithm.hpp"

/*
пусть n - размер хеша в битах
размер исходных данных - это размер хеша
открытый ключ - это n*2 хешей
закрытый ключ - это n*2 исходных ключей (такого же размера, как и хеши)
подпись - это n исходных ключей
*/

LamportSignatureAlgorithm::LamportSignatureAlgorithm(ptr<HashAlgorithm> hashAlgorithm)
: hashAlgorithm(hashAlgorithm), hashSize(hashAlgorithm->GetHashSize())
{
}

size_t LamportSignatureAlgorithm::GetDataSize() const
{
	return hashSize;
}

size_t LamportSignatureAlgorithm::GetPublicKeySize() const
{
	return hashSize * 8 * 2 * hashSize;
}

size_t LamportSignatureAlgorithm::GetPrivateKeySize() const
{
	return hashSize * 8 * 2 * hashSize;
}

size_t LamportSignatureAlgorithm::GetSignatureSize() const
{
	return hashSize * 8 * hashSize;
}

void LamportSignatureAlgorithm::GenerateKeyPair(ptr<RandomAlgorithm> randomAlgorithm, void* privateKey, void* publicKey)
{
	// сгенерировать случайные данные в качестве закрытого ключа
	randomAlgorithm->GenerateRandom(privateKey, GetPrivateKeySize());
	// сделать хеши закрытого ключа в качестве открытого ключа
	ptr<HashStream> hashStream = hashAlgorithm->CreateHashStream();
	for(size_t i = 0; i < hashSize * 8 * 2; ++i)
	{
		hashStream->Reset();
		hashStream->Write((char*)privateKey + i * hashSize, hashSize);
		hashStream->End();
		hashStream->GetHash((char*)publicKey + i * hashSize);
	}
}

void LamportSignatureAlgorithm::Sign(const void* data, const void* privateKey, void* signature)
{
	const unsigned char* dataBytes = (const unsigned char*)data;

	for(size_t i = 0; i < hashSize * 8; ++i)
		memcpy((char*)signature + i * hashSize, (char*)privateKey + (i * 2 + ((dataBytes[i / 8] >> (i % 8)) & 1)) * hashSize, hashSize);
}

bool LamportSignatureAlgorithm::Verify(const void* data, const void* publicKey, const void* signature)
{
	const unsigned char* dataBytes = (const unsigned char*)data;

	unsigned char* tempHash = (unsigned char*)alloca(hashSize);

	ptr<HashStream> hashStream = hashAlgorithm->CreateHashStream();
	for(size_t i = 0; i < hashSize * 8; ++i)
	{
		hashStream->Reset();
		hashStream->Write((char*)signature + i * hashSize, hashSize);
		hashStream->End();
		hashStream->GetHash(tempHash);
		if(memcmp((char*)publicKey + (i * 2 + ((dataBytes[i / 8] >> (i % 8)) & 1)) * hashSize, tempHash, hashSize) != 0)
			return false;
	}
	return true;
}
