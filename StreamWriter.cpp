#include "StreamWriter.hpp"
#include "OutputStream.hpp"
#include "Exception.hpp"
#if defined(___INANITY_PLATFORM_LINUX)
#include <alloca.h>
#elif defined(___INANITY_PLATFORM_FREEBSD)
#include <stdlib.h>
#endif

BEGIN_INANITY

StreamWriter::StreamWriter(ptr<OutputStream> stream) : stream(stream)
{
}

bigsize_t StreamWriter::GetWrittenSize() const
{
	return written;
}

void StreamWriter::Write(const void* data, size_t size)
{
	stream->Write(data, size);
	written += size;
}

void StreamWriter::WriteShortly(size_t data)
{
	WriteShortlyBig(data);
}

/*
Формат сокращенных чисел:
Длина сокращенного числа определяется по первому байту, а точнее, по его
нескольким старшим битам.
0 - длина равна байту.
10 - длина равна 2 байтам.
110 - длина равна 3 байтам.
1110 - длина равна 4 байтам.
11110 - длина равна 5 байтам.
111110 - длина равна 6 байтам.
1111110 - длина равна 7 байтам.
11111110 - длина равна 8 байтам.
11111111 - длина равна 9 байтам.
При этом в первом байте записывается старший байт числа, а в последующих -
оставшиеся байты числа, в порядке big-endian.
Максимальное представимое число - 64-битное.
*/

void StreamWriter::WriteShortlyBig(bigsize_t data)
{
	// additional length
	int length;
	// bytes
	uint8_t bytes[9];

	if(data < 0x80)
	{
		length = 0;
		bytes[0] = 0x00;
	}
	else if(data < 0x4000)
	{
		length = 1;
		bytes[0] = 0x80;
	}
	else if(data < 0x200000)
	{
		length = 2;
		bytes[0] = 0xC0;
	}
	else if(data < 0x10000000)
	{
		length = 3;
		bytes[0] = 0xE0;
	}
	// eliminate warnings about always-true comparison
	// in case of small bigsize_t
#ifdef ___INANITY_BIGSIZE_IS_BIG
	else if(data < 0x800000000ULL)
	{
		length = 4;
		bytes[0] = 0xF0;
	}
	else if(data < 0x40000000000ULL)
	{
		length = 5;
		bytes[0] = 0xF8;
	}
	else if(data < 0x2000000000000ULL)
	{
		length = 6;
		bytes[0] = 0xFC;
	}
	else if(data < 0x1000000000000ULL)
	{
		length = 7;
		bytes[0] = 0xFE;
	}
	else
	{
		length = 8;
		bytes[0] = 0xFF;
	}
#else
	else
	{
		length = 4;
		bytes[0] = 0xF0;
	}
#endif

	// prepare first byte
	bytes[0] |= (uint8_t)(data >> (length * 8));
	// prepare additional bytes
	for(int i = 0; i < length; ++i)
		bytes[1 + i] = (uint8_t)(data >> ((length - 1 - i) * 8));

	// write
	Write(bytes, length + 1);
}

void StreamWriter::WriteString(const String& data)
{
	//сначала записать длину строки
	WriteShortly(data.length());
	//затем саму строку
	if(data.length())
		Write(data.c_str(), data.length());
}

void StreamWriter::WriteGap(size_t alignment)
{
#ifdef _DEBUG
	//проверить, что выравнивание - степень двойки
	if(alignment & (alignment - 1))
		THROW("Alignment must be power of two");
#endif

	//вычислить количество байт
	alignment = alignment - ((written - 1) & (alignment - 1)) - 1;
	//записать их
	if(alignment)
	{
		uint8_t* data = (uint8_t*)alloca(alignment);
		for(size_t i = 0; i < alignment; ++i)
			data[i] = 0xCC;
		Write(data, alignment);
	}
}

END_INANITY
