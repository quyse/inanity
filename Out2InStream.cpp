#include "Out2InStream.hpp"
#include "File.hpp"
#include "CriticalCode.hpp"
#include "Exception.hpp"
#include <cstring>

BEGIN_INANITY

Out2InStream::Reader::Reader(ptr<Out2InStream> stream)
	: stream(stream), firstOffset(0)
{
	// зарегистрировать считыватель в потоке
	CriticalCode code(stream->criticalSection);
	stream->readers.push_front(this);
	iterator = stream->readers.begin();
}

Out2InStream::Reader::~Reader()
{
	// разрегистрировать считыватель в потоке
	CriticalCode code(stream->criticalSection);
	stream->readers.erase(iterator);
}

size_t Out2InStream::Reader::Read(void* data, size_t size)
{
	char* dataPtr = (char*)data;
	// цикл по кускам, которыми будем считывать
	while(size)
	{
		// захватить семафор
		semaphore.Acquire();
		// теперь мы знаем, что в очереди есть как минимум один файл
		CriticalCode code(criticalSection);
		// получить первый файл
		ptr<File> file = files.front();
		// если файл - нулевой, это означает, что достигнут конец потока
		if(!file)
		{
			// освободить семафор обратно
			semaphore.Release();
			// преждевременно выйти
			break;
		}
		size_t fileSize = file->GetSize();
		// получить размер куска, который мы возьмём из этого файла
		size_t chunkSize = std::min(file->GetSize() - firstOffset, size);
		// скопировать данные
		memcpy(dataPtr, (char*)file->GetData() + firstOffset, chunkSize);
		dataPtr += chunkSize;
		size -= chunkSize;
		// передвинуть указатель
		firstOffset += chunkSize;
		// если указатель достиг конца файла, перейти к следующему файлу
		if(firstOffset >= fileSize)
		{
			files.pop();
			firstOffset = 0;
		}
		// а иначе файл остаётся, и нужно вернуть семафору единицу счётчика
		else
			semaphore.Release();
	}
	return dataPtr - (char*)data;
}

void Out2InStream::Reader::Push(ptr<File> file)
{
	CriticalCode code(criticalSection);
	// добавить файл
	files.push(file);
	// сообщить о новом файле
	semaphore.Release();
}

Out2InStream::Out2InStream() : flushed(false)
{
}

void Out2InStream::WriteFile(ptr<File> file)
{
	// просто передать файл всем читателям
	CriticalCode code(criticalSection);
	if(flushed)
		THROW("Out2InStream already flushed");
	for(std::list<Reader*>::const_iterator i = readers.begin(); i != readers.end(); ++i)
		(*i)->Push(file);
}

void Out2InStream::End()
{
	// просто передать всем
	CriticalCode code(criticalSection);
	if(flushed)
		THROW("Out2InStream already flushed");
	flushed = true;
	for(std::list<Reader*>::const_iterator i = readers.begin(); i != readers.end(); ++i)
		(*i)->Push(0);
}

ptr<InputStream> Out2InStream::CreateInputStream()
{
	// создать считыватель
	ptr<Reader> reader = NEW(Reader(this));
	// если поток уже закончился, сразу выдать сигнал
	CriticalCode code(criticalSection);
	if(flushed)
		reader->Push(0);
	return reader;
}

END_INANITY
