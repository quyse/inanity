#include "DecompressStream.hpp"
#include "MemoryFile.hpp"
#include "FileInputStream.hpp"
#include "MemoryStream.hpp"
#include "Exception.hpp"
#include <memory.h>

BEGIN_INANITY

DecompressStream::DecompressStream(ptr<InputStream> inputStream)
: inputFile(NEW(MemoryFile(inputBufferSize))), inputStream(inputStream), finished(false)
{
	try
	{
		//инициализировать поток zlib
		zstream.zalloc = Z_NULL;
		zstream.zfree = Z_NULL;
		zstream.opaque = Z_NULL;
		switch(inflateInit(&zstream))
		{
		case Z_OK:
			break;
		default:
			THROW("Can't initialize inflation");
		}

		//выделить память под выходной буфер
		outputFile = NEW(MemoryFile(outputBufferSize));

		//указать параметры буферов для потока
		zstream.next_in = (Bytef*)inputFile->GetData();
		zstream.avail_in = 0;
		zstream.next_out = (Bytef*)outputFile->GetData();
		zstream.avail_out = outputBufferSize;
		//установить начало данных на начало буфера
		outputData = zstream.next_out;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't initialize compress stream", exception);
	}
}

DecompressStream::~DecompressStream()
{
	inflateEnd(&zstream);
}

/*
Между вызовами Read и между итерациями цикла внутри Read поддерживается
следующий инвариант.
Входной буфер - может содержать данные
next_in - начало входного буфера
avail_in - размер этих данных
Выходной буфер - также может содержать данные
next_out - конец этих данных
avail_out - размер оставшегося места в буфере (то есть без данных)
outputData - начало этих данных (то есть еще не считанных)
*/
size_t DecompressStream::Read(void* data, size_t size)
{
	try
	{
		size_t allSize = 0;

		//запомнить параметры буферов
		Bytef* inputBuffer = zstream.next_in;

		//пока есть данные для чтения
		while(size)
		{
			//если в выходном буфере есть данные
			if(outputData < zstream.next_out)
			{
				//скопировать, сколько нужно
				unsigned toWrite = (unsigned)std::min<size_t>(size, zstream.next_out - outputData);
				memcpy(data, outputData, toWrite);
				outputData += toWrite;
				data = (char*)data + toWrite;
				size -= toWrite;
				allSize += toWrite;
			}
			//иначе выходной буфер пуст
			//если данных больше нет
			else if(finished)
				//завершить считывание
				return allSize;
			//иначе данные еще могут быть
			else
			{
				//приписать данные к концу входного буфера, чтобы заполнить его
				zstream.avail_in += (unsigned)inputStream->Read(zstream.next_in + zstream.avail_in, inputBufferSize - zstream.avail_in);

				//подготовить выходной буфер к приему распакованных данных
				zstream.next_out = (Bytef*)outputFile->GetData();
				zstream.avail_out = outputBufferSize;
				//установить указатель выходных данных на начало буфера
				outputData = zstream.next_out;

				//выполнить распаковку данных
				switch(inflate(&zstream, Z_NO_FLUSH))
				{
				case Z_OK:
				case Z_BUF_ERROR:
					break;
				case Z_STREAM_END:
					//установить флажок конца
					finished = true;
					break;
				default:
					THROW("Decompression error");
				}

				//перенести данные, которые остались во входном буфере, в его начало
				memmove(inputBuffer, zstream.next_in, zstream.avail_in);
				zstream.next_in = inputBuffer;
			}
		}

		return allSize;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't decompress data", exception);
	}
}

ptr<File> DecompressStream::DecompressFile(ptr<File> file)
{
	try
	{
		//создать поток для распаковки
		ptr<InputStream> stream = NEW(DecompressStream(NEW(FileInputStream(file))));
		//создать выходной поток
		ptr<MemoryStream> outputStream = NEW(MemoryStream);

		//выделить буфер
		const unsigned bufferSize = 0x1000;
		char buffer[bufferSize];

		//цикл распаковки
		while(size_t read = stream->Read(buffer, bufferSize))
			outputStream->Write(buffer, read);
		return outputStream->ToFile();
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't decompress to file", exception);
	}
}

END_INANITY
