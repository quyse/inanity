#ifndef ___INANITY_COMPRESS_STREAM_HPP___
#define ___INANITY_COMPRESS_STREAM_HPP___

#include "OutputStream.hpp"
#include "scripting_decl.hpp"
#include "zlib.hpp"

/*
Файл содержит класс потока для сжатия данных.
*/

BEGIN_INANITY

class File;
class CompressStream : public OutputStream
{
public:
	//уровни компрессии
	enum CompressionLevel
	{
		compressionNo = 0,
		compressionMin = 1,
		compressionMax = 9,
		compressionDefault = -1
	};

private:
	//размер буфера для входных данных
	static const unsigned inputBufferSize = 0x10000;

	//входной буфер
	ptr<File> inputFile;
	//выходной буфер
	ptr<File> outputFile;
	//ZLIB-поток
	z_stream zstream;
	//выходной поток
	ptr<OutputStream> outputStream;

	//завершена ли запись
	bool finalized;

	//выполнить запись выходного буфера в выходной поток
	void WriteOutput(void* data, size_t size);

public:
	CompressStream(ptr<OutputStream> outputStream, CompressionLevel compressionLevel);
	~CompressStream();

	static ptr<File> CompressFile(ptr<File> file, CompressionLevel compressionLevel);

	void Write(const void* data, size_t size);
	void Flush();

	static ptr<CompressStream> CreateMax(ptr<OutputStream> outputStream);

	SCRIPTABLE_CLASS(CompressStream);
};

END_INANITY

#endif
