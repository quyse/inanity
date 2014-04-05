#include "StreamReader.hpp"
#include "InputStream.hpp"
#include "Exception.hpp"
#if defined(___INANITY_PLATFORM_LINUX)
#include <alloca.h>
#elif defined(___INANITY_PLATFORM_FREEBSD)
#include <stdlib.h>
#endif

BEGIN_INANITY

StreamReader::StreamReader(ptr<InputStream> stream) : stream(stream), read(0)
{
}

bigsize_t StreamReader::GetReadSize() const
{
	return read;
}

size_t StreamReader::Read(void* data, size_t size)
{
	if(stream->Read(data, size) != size)
		THROW("Can't read data");
	read += size;
	return size;
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

size_t StreamReader::ReadShortly()
{
	bigsize_t a = ReadShortlyBig();
	size_t b = (size_t)a;
	if(a != b)
		THROW("Can't read shortly because number is too big");
	return b;
}

/*
Формат сокращенных чисел, см. примечание к реализации StreamWriter::WriteShortly().
*/

bigsize_t StreamReader::ReadShortlyBig()
{
	// read first byte
	unsigned char first = Read<unsigned char>();
	// determine additional length
	int length;
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

	// read additional bytes
	unsigned char bytes[8];
	Read(bytes, length);

	// calculate result
	bigsize_t result = ((bigsize_t)first) << (length * 8);
	for(int i = 0; i < length; ++i)
		result |= ((bigsize_t)bytes[i]) << ((length - 1 - i) * 8);

	return result;
}

void StreamReader::ReadGap(size_t alignment)
{
#ifdef _DEBUG
	//проверить, что выравнивание - степень двойки
	if(alignment & (alignment - 1))
		THROW("Alignment must be power of two");
#endif

	//вычислить количество байт
	alignment = alignment - ((read - 1) & (alignment - 1)) - 1;
	//считать их
	if(alignment)
	{
		char* data = (char*)alloca(alignment);
		Read(data, alignment);
	}
}

void StreamReader::ReadEnd()
{
	char c;
	if(stream->Read(&c, 1) != 0)
		THROW("Expected end of stream");
}

bigsize_t StreamReader::Skip(bigsize_t size)
{
	bigsize_t skipped = stream->Skip(size);
	if(skipped != size)
		THROW("Not enough data to skip");
	return skipped;
}

END_INANITY
