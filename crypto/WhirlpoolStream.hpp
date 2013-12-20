#ifndef ___INANITY_CRYPTO_WHIRLPOOL_STREAM_HPP___
#define ___INANITY_CRYPTO_WHIRLPOOL_STREAM_HPP___

#include "HashStream.hpp"

BEGIN_INANITY_CRYPTO

/// Класс, вычисляющий хеш-функцию Whirlpool.
/** Основан на референсной реализации, находящейся в public domain.
http://www.larc.usp.br/~pbarreto/WhirlpoolPage.html
*/
class WhirlpoolStream : public HashStream
{
public:
	static const size_t hashSize;
	static const char* algorithmName;

private:
	unsigned char bitLength[32]; /* global number of hashed bits (256-bit counter) */
	unsigned char buffer[64];	/* buffer of data to hash */
	int bufferBits;		        /* current number of bits on the buffer */
	int bufferPos;		        /* current (possibly incomplete) byte slot on the buffer */
	unsigned long long hash[8];    /* the hashing state */
	/// Результирующий хеш.
	unsigned char digest[64];
	/// Завершён ли буфер.
	bool ended;

	void processBuffer();
	void add(const unsigned char * const source, unsigned long sourceBits);

public:
	WhirlpoolStream();

	void Write(const void* data, size_t size);

	void End();
	size_t GetHashSize() const;
	void GetHash(void* data) const;
	void Reset();
};

END_INANITY_CRYPTO

#endif
