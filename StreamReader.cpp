#include "StreamReader.hpp"
#include "InputStream.hpp"
#include "Exception.hpp"
#ifdef ___INANITY_LINUX
#include <alloca.h>
#endif

StreamReader::StreamReader(ptr<InputStream> stream) : stream(stream), read(0)
{
}

size_t StreamReader::GetReadSize() const
{
	return read;
}

void StreamReader::Read(void* data, size_t size)
{
	if(stream->Read(data, size) != size)
		THROW_PRIMARY_EXCEPTION("Can't read data");
	read += size;
}

/*
Формат сокращенных чисел, см. примечание к реализации StreamWriter::WriteShortly().
*/

unsigned StreamReader::ReadShortly()
{
	//считать первый байт
	unsigned first = Read<unsigned char>();
	//определить длину дополнительной части числа
	unsigned length;
	if(!(first & 0x80))
		//дополнительной части нет, можно сразу вернуть ответ
		return first;
	else if(!(first & 0x40))
	{
		length = 1;
		first &= ~0x80;
	}
	else if(!(first & 0x20))
	{
		length = 2;
		first &= ~0xC0;
	}
	else if(!(first & 0x10))
	{
		length = 3;
		first &= ~0xE0;
	}
	else
	{
		length = 4;
		first &= ~0xF0;
	}
	//считать дополнительную часть числа
	unsigned a = 0;
	Read(&a, length);
	//вернуть результат
	return a | (first << (length * 8));
}

void StreamReader::ReadGap(unsigned alignment)
{
#ifdef _DEBUG
	//проверить, что выравнивание - степень двойки
	if(alignment & (alignment - 1))
		THROW_PRIMARY_EXCEPTION("Alignment must be power of two");
#endif

	//вычислить количество байт
	alignment = alignment - ((read - 1) & (alignment - 1)) - 1;
	//считать их
	if(alignment)
	{
		unsigned char* data = (unsigned char*)alloca(alignment);
		Read(data, alignment);
	}
}
