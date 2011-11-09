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

String StreamReader::ReadString()
{
	//считать длину строки
	size_t length = ReadShortly();
	//считать строку
	std::string r(length, ' ');
	if(length)
		Read(&*r.begin(), length);
	return r;
}

/*
Формат сокращенных чисел, см. примечание к реализации StreamWriter::WriteShortly().
*/

size_t StreamReader::ReadShortly()
{
	//считать первый байт
	size_t first = Read<unsigned char>();
	//определить длину дополнительной части числа
	size_t length;
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
	else if(!(first & 0x08))
	{
		length = 4;
		first &= ~0xF0;
	}
	else if(!(first & 0x04))
	{
		length = 5;
		first &= ~0xF8;
	}
	else if(!(first & 0x02))
	{
		length = 6;
		first &= ~0xFC;
	}
	else if(!(first & 0x01))
	{
		length = 7;
		first &= ~0xFE;
	}
	else
	{
		length = 8;
		first &= ~0xFF;
	}
	//считать дополнительную часть числа
	unsigned long long a = 0;
	Read(&a, length);
	//вернуть результат
	return (size_t)(a | (first << (length * 8)));
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
