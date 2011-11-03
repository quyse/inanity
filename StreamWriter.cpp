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
При этом в первом байте записывается старший байт числа, а в последующих -
оставшиеся байты числа, в порядке x86. То есть весь формат выглядит, как
low-endian, с переставленным на первое место старшим байтом.
*/

void StreamWriter::WriteShortly(unsigned data)
{
	//длина числа (без первого байта)
	unsigned length;
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
	else
	{
		length = 4;
		first = 0xF0;
	}

	//добавить в первый байт старший байт числа, и записать первый байт
	Write<unsigned char>(first | data >> (length * 8));
	//записать остальное число
	Write(&data, length);
}

void StreamWriter::Write(const std::wstring& data)
{
	//сначала записать длину строки
	WriteShortly((unsigned)data.length());
	//затем саму строку
	Write(data.c_str(), data.length() * sizeof(wchar_t));
}

void StreamWriter::Write(const std::string& data)
{
	//сначала записать длину строки
	WriteShortly((unsigned)data.length());
	//затем саму строку
	Write(data.c_str(), data.length());
}

void StreamWriter::WriteGap(unsigned alignment)
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
		for(unsigned i = 0; i < alignment; ++i)
			data[i] = 0xCC;
		Write(data, alignment);
	}
}
