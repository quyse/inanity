#ifndef ___INANITY_CRYPTO_STREAM_HASHER_HPP___
#define ___INANITY_CRYPTO_STREAM_HASHER_HPP___

#include "crypto.hpp"
#include "../InputStream.hpp"
#include "../OutputStream.hpp"

BEGIN_INANITY_CRYPTO

class HashAlgorithm;
class HashStream;

/// Класс, занимающийся защищёнными от повреждений потоками.
/** Позволяет создавать потоки, создающие хеш-цепочки, или
наоборот, проверящие потоки на соответствие хеш-цепочкам. */
class StreamHasher : public Object
{
private:
	/// Алгоритм хеширования.
	ptr<HashAlgorithm> hashAlgorithm;
	/// Размер блока.
	/** Такими блоками данные хешируются. */
	size_t blockSize;

public:
	/// Создать хешер.
	StreamHasher(ptr<HashAlgorithm> hashAlgorithm, size_t blockSize);

	/// Создать поток хеширования.
	/** Он принимает данные, а выдаёт цепочку хешей. */
	ptr<OutputStream> CreateHasherStream(ptr<OutputStream> destStream);

	/// Создать поток проверки.
	/** Он принимает поток данных и поток хешей, а возвращает только проверенные данные.
	Если данные оказываются повреждёнными, выбрасывается исключение при чтении. */
	ptr<InputStream> CreateVerifyStream(ptr<InputStream> sourceDataStream, ptr<InputStream> sourceHashStream);

private:
	/// Класс потока хеширования.
	class HasherStream : public OutputStream
	{
	private:
		/// Поток хеширования.
		ptr<HashStream> hashStream;
		/// Размер хеша.
		size_t hashSize;
		/// Размер блока.
		size_t blockSize;
		/// Результирующий поток.
		ptr<OutputStream> destStream;

		/// Буфер данных блока.
		ptr<File> blockFile;
		char* block;
		/// Количество накопленных данных в текущем блоке.
		size_t blockDataSize;

		/// Записать блок.
		void WriteBlock();

	public:
		HasherStream(ptr<StreamHasher> hasher, ptr<OutputStream> destStream);

		void Write(const void* data, size_t size);
		void Flush();
	};

	/// Класс потока проверки.
	class VerifyStream : public InputStream
	{
	private:
		/// Поток хеширования.
		ptr<HashStream> hashStream;
		/// Размер хеша.
		size_t hashSize;
		/// Размер блока.
		size_t blockSize;
		/// Поток данных.
		ptr<InputStream> sourceDataStream;
		/// Поток хешей.
		ptr<InputStream> sourceHashStream;

		/// Буфер данных блока.
		ptr<File> blockFile;
		char* block;
		/// Количество уже считанных данных в текущем блоке.
		size_t blockReadSize;
		/// Количество данных в блоке.
		/** В основном оно равно размеру блока, кроме самого последнего блока. */
		size_t blockDataSize;

		/// Считать очередной блок.
		void ReadBlock();

	public:
		VerifyStream(ptr<StreamHasher> hasher, ptr<InputStream> sourceDataStream, ptr<InputStream> sourceHashStream);

		size_t Read(void* data, size_t size);
	};
};

END_INANITY_CRYPTO

#endif
