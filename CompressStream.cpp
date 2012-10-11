#include "CompressStream.hpp"
#include "MemoryFile.hpp"
#include "MemoryStream.hpp"
#include "Exception.hpp"
#include <memory.h>

CompressStream::CompressStream(ptr<OutputStream> outputStream, CompressionLevel compressionLevel)
: outputStream(outputStream), inputFile(NEW(MemoryFile(inputBufferSize))), finalized(false)
{
	try
	{
		//инициализировать поток zlib
		zstream.zalloc = Z_NULL;
		zstream.zfree = Z_NULL;
		zstream.opaque = Z_NULL;
		switch(deflateInit(&zstream, compressionLevel))
		{
		case Z_OK:
			break;
		case Z_STREAM_ERROR:
			THROW_PRIMARY_EXCEPTION("Invalid compression level");
		default:
			THROW_PRIMARY_EXCEPTION("Can't initialize deflation");
		}

		//выделить память под выходной буфер
		unsigned outputBound = deflateBound(&zstream, inputBufferSize);
		outputFile = NEW(MemoryFile(outputBound));

		//указать параметры буферов для потока
		zstream.next_in = (Bytef*)inputFile->GetData();
		zstream.avail_in = 0;
		zstream.next_out = (Bytef*)outputFile->GetData();
		zstream.avail_out = outputBound;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't initialize compress stream", exception);
	}
}

CompressStream::~CompressStream()
{
	//освободить ресурсы; мы не сбрасываем текущие данные из буфера,
	//так как это может вызвать исключение
	//деструктор здесь только предупреждает утечку ресурсов
	if(!finalized)
		deflateEnd(&zstream);
}

void CompressStream::WriteOutput(void* data, size_t size)
{
	//записать данные в выходной поток
	outputStream->Write(data, size);
}

/*
Между вызовами Write и между итерациями цикла внутри Write поддерживается
следующий инвариант.
Входной буфер - может содержать данные
next_in - начало входного буфера
avail_in - размер этих данных
Выходной буфер - всегда пустой
next_out - начало выходного буфера
avail_out - размер выходного буфера
*/
void CompressStream::Write(const void* data, size_t size)
{
	try
	{
		//если финализация уже была выполнена, то записывать данные нельзя
		if(finalized)
			THROW_PRIMARY_EXCEPTION("Stream already finalized");

		//запомнить параметры буферов
		Bytef* inputBuffer = zstream.next_in;
		Bytef* outputBuffer = zstream.next_out;
		unsigned outputSize = zstream.avail_out;

		//пока есть данные
		while(size)
		{
			//приписать данные к концу входного буфера
			unsigned toRead = (unsigned)std::min<size_t>(size, inputBufferSize - zstream.avail_in);
			memcpy(zstream.next_in + zstream.avail_in, data, toRead);
			zstream.avail_in += toRead;
			data = (char*)data + toRead;
			size -= toRead;
			//если входной буфер переполнен
			if(zstream.avail_in >= inputBufferSize)
			{
				//выполнить сжатие данных
				switch(deflate(&zstream, Z_NO_FLUSH))
				{
				case Z_OK:
				case Z_BUF_ERROR:
					break;
				default:
					THROW_PRIMARY_EXCEPTION("Compression error");
				}

				//записать данные в выходной поток
				WriteOutput(outputBuffer, outputSize - zstream.avail_out);

				//перенести данные, которые остались во входном буфере, в его начало
				memmove(inputBuffer, zstream.next_in, zstream.avail_in);
				zstream.next_in = inputBuffer;
				//очистить выходной буфер
				zstream.next_out = outputBuffer;
				zstream.avail_out = outputSize;
			}
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't compress data", exception);
	}
}

/*
Во время и после Flush указанный для Write инвариант не выполняется.
*/
void CompressStream::Flush()
{
	try
	{
		//если финализация уже была выполнена, закончить
		if(finalized) return;

		//запомнить параметры выходного буфера
		Bytef* outputBuffer = zstream.next_out;
		unsigned outputSize = zstream.avail_out;

		//цикл, пока компрессия не завершена
		for(;;)
		{
			//выполнять компрессию
			int result = deflate(&zstream, Z_FINISH);

			//обработка ошибок
			switch(result)
			{
			case Z_STREAM_END:
			case Z_OK:
				break;
			default:
				THROW_PRIMARY_EXCEPTION("Compression error");
			}

			//записать вывод в поток
			WriteOutput(outputBuffer, outputSize - zstream.avail_out);
			//очистить выходной буфер
			zstream.next_out = outputBuffer;
			zstream.avail_out = outputSize;

			//если это конец, закончить
			if(result == Z_STREAM_END)
				break;
		}

		//завершить сжатие
		if(deflateEnd(&zstream) != Z_OK)
			THROW_PRIMARY_EXCEPTION("Finalize stream error");

		//установить флажок завершенности
		finalized = true;

		//выполнить финализацию потока
		outputStream->Flush();
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't finalize compression", exception);
	}
}

ptr<File> CompressStream::CompressFile(ptr<File> file, CompressionLevel compressionLevel)
{
	try
	{
		//создать выходной поток
		ptr<MemoryStream> outputStream = NEW(MemoryStream);
		//создать поток для сжатия
		ptr<OutputStream> stream = NEW(CompressStream(&*outputStream, compressionLevel));

		//сжать данные
		stream->Write(file->GetData(), file->GetSize());
		stream->Flush();
		//вернуть файл
		return outputStream->ToFile();
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't decompress to file", exception);
	}
}
