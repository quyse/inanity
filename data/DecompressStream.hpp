#ifndef ___INANITY_DATA_DECOMPRESS_STREAM_HPP___
#define ___INANITY_DATA_DECOMPRESS_STREAM_HPP___

#include "data.hpp"
#include "../InputStream.hpp"
#include "../meta/decl.hpp"
#include "../zlib.hpp"

/*
Файл содержит класс потока для распаковки данных.
*/

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_DATA

class DecompressStream : public InputStream
{
private:
	//размер буфера для входных данных
	static const unsigned inputBufferSize = 0x10000;
	//размер буфера для выходных данных
	static const unsigned outputBufferSize = 0x80000;

	//входной буфер
	ptr<File> inputFile;
	//выходной буфер
	ptr<File> outputFile;
	//ZLIB-поток
	z_stream zstream;
	//начало еще не переданных данных в выходном буфере
	Bytef* outputData;
	//входной поток
	ptr<InputStream> inputStream;
	//достигнут конец
	bool finished;

public:
	DecompressStream(ptr<InputStream> inputStream);
	~DecompressStream();

	static ptr<File> DecompressFile(ptr<File> file);

	size_t Read(void* data, size_t size);

	META_DECLARE_CLASS(DecompressStream);
};

END_INANITY_DATA

#endif
