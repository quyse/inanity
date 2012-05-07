#ifndef ___INANITY_CRYPTO_STREAM_SIGNER_HPP___
#define ___INANITY_CRYPTO_STREAM_SIGNER_HPP___

#include "crypto.hpp"
#include "../InputStream.hpp"

BEGIN_INANITY

class File;
class OutputStream;

END_INANITY

BEGIN_INANITY_CRYPTO

class SignatureAlgorithm;
class HashAlgorithm;

/// Класс, защищающий данные от модификации.
/**
Формат потока данных:
Заголовок.
Цепочка хешей.
Цифровая подпись цепочки хешей.
Данные.

Какие используются алгоритмы хеширования (их два - для блоков, и
для цифровой подписи), в потоке не сохраняется.

Класс оптимизирован для быстрой распаковки, а не запаковки.
Распаковка выполняется в один проход потока, запаковка - вообще
не потоком, а файлом.
*/
class StreamSigner : public Object
{
private:
	/// Алгоритм хеширования блоков.
	ptr<HashAlgorithm> blockHashAlgorithm;
	/// Алгоритм хеширования для подписи.
	ptr<HashAlgorithm> signatureHashAlgorithm;
	/// Алгоритм цифровой подписи.
	ptr<SignatureAlgorithm> signatureAlgorithm;
	/// Размер блока для хеширования.
	size_t blockSize;

public:
	StreamSigner(ptr<HashAlgorithm> blockHashAlgorithm, ptr<HashAlgorithm> signatureHashAlgorithm, ptr<SignatureAlgorithm> signatureAlgorithm, size_t blockSize);

	/// Записать заголовок запакованного потока.
	/** Метод считывает выданный поток исходных данных. Данные следует записывать
	в результирующий поток сразу после заголовка. */
	void WriteSigningHeader(ptr<InputStream> sourceStream, ptr<OutputStream> destStream, ptr<File> privateKey);
	/// Создать поток распаковки.
	ptr<InputStream> CreateVerifyStream(ptr<InputStream> sourceStream, ptr<File> publicKey);

private:
	/// Класс потока распаковки.
	class VerifyStream : public InputStream
	{
	private:
		ptr<StreamSigner> signer;
		ptr<InputStream> sourceStream;
		ptr<File> publicKey;
		/// Поток StreamHasher::VerifyStream.
		/** Сначала заголовок ещё не считан, тогда тут nullptr. */
		ptr<InputStream> verifyStream;

		/// Считать заголовок.
		void ReadHeader();

	public:
		VerifyStream(ptr<StreamSigner> signer, ptr<InputStream> sourceStream, ptr<File> publicKey);

		size_t Read(void* data, size_t size);
	};
};

END_INANITY_CRYPTO

#endif
