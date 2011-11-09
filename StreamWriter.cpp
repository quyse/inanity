#include "StreamWriter.hpp"
#include "OutputStream.hpp"
#include "Exception.hpp"
#ifdef ___INANITY_LINUX
#include <alloca.h>
#endif

StreamWriter::StreamWriter(ptr<OutputStream> stream) : stream(stream), written(0)
{
}

size_t StreamWriter::GetWrittenSize() const
{
	return written;
}

void StreamWriter::Write(const void* data, size_t size)
{
	stream->Write(data, size);
	written += size;
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
оставшиеся байты числа, в порядке x86. То есть весь формат выглядит, как
low-endian, с переставленным на первое место старшим байтом.
Максимальное представимое число - 64-битное, но оно обрезается но размера size_t.
*/

void StreamWriter::WriteShortly(size_t data)
{
	//длина числа (без первого байта)
	size_t length;
	//первый байт
	unsigned char first;

	if(data < 0x80)
	{
		length = 0;
		first = 0x00;
	}
	else if(data < 0x4000)
	{
		length = 1;
		first = 0x80;
	}
	else if(data < 0x200000)
	{
		length = 2;
		first = 0xC0;
	}
	else if(data < 0x10000000)
	{
		length = 3;
		first = 0xE0;
	}
	else if(data < 0x800000000ULL)
	{
		length = 4;
		first = 0xF0;
	}
	else if(data < 0x40000000000ULL)
	{
		length = 5;
		first = 0xF8;
	}
	else if(data < 0x2000000000000ULL)
	{
		length = 6;
		first = 0xFC;
	}
	else if(data < 0x1000000000000ULL)
	{
		length = 7;
		first = 0xFE;
	}
	else
	{
		length = 8;
		first = 0xFF;
	}

	//добавить в первый байт старший байт числа, и записать первый байт
	Write<unsigned char>(first | data >> (length * 8));
	//записать остальное число
	Write(&data, length);
}

void StreamWriter::WriteString(const String& data)
{
	//сначала записать длину строки
	WriteShortly(data.length());
	//затем саму строку
	Write(data.c_str(), data.length());
}

void StreamWriter::WriteGap(size_t alignment)
{
#ifdef _DEBUG
	//проверить, что выравнивание - степень двойки
	if(alignment & (alignment - 1))
		THROW_PRIMARY_EXCEPTION("Alignment must be power of two");
#endif

	//вычислить количество байт
	alignment = alignment - ((written - 1) & (alignment - 1)) - 1;
	//записать их
	if(alignment)
	{
		unsigned char* data = (unsigned char*)alloca(alignment);
		for(size_t i = 0; i < alignment; ++i)
			data[i] = 0xCC;
		Write(data, alignment);
	}
}

void StreamWriter::Flush()
{
	stream->Flush();
}
