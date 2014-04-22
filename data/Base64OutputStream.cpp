#include "Base64OutputStream.hpp"
#include "../MemoryStream.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_DATA

const char Base64OutputStream::schema[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
unsigned char Base64OutputStream::inverseSchema[256];

Base64OutputStream::Base64OutputStream(bool encoding, ptr<OutputStream> outputStream) :
	encoding(encoding), outputStream(outputStream), buffer(0), bufferSize(24), flushed(false)
{
	// если декодирование, инициализировать обратную схему, если ещё не сделано
	if(!encoding && inverseSchema[0] != 0xFF)
	{
		for(int i = 0; i < 256; ++i)
			inverseSchema[i] = 0xFF;
		for(int i = 0; i < 64; ++i)
			inverseSchema[(ptrdiff_t)schema[i]] = i;
	}
}

Base64OutputStream::~Base64OutputStream()
{
	Flush();
}

void Base64OutputStream::WriteEncode(unsigned char byte)
{
	// добавить байт в буфер, начиная со старших байт
	bufferSize -= 8;
	buffer |= byte << bufferSize;
	// выполнить обработку
	ProcessEncodeBuffer();
}

void Base64OutputStream::WriteDecode(unsigned char byte)
{
	if(byte == '=')
		return;
	// преобразовать байт
	byte = inverseSchema[byte];
	// если байт известный
	if(byte < 64)
	{
		// добавить в буфер, начиная со старших байт
		bufferSize -= 6;
		buffer |= byte << bufferSize;
		// выполнить обработку
		ProcessDecodeBuffer();
	}
	else
		THROW("Invalid byte");
}

void Base64OutputStream::ProcessEncodeBuffer()
{
	// если буфер заполнен, или объявлен конец
	if(!bufferSize || flushed)
	{
		// выдать столько символов base64, сколько есть
		// есть свободно
		// 0    24
		// 8    16 ==
		// 16   8  =
		// 24   0
		int bufferSizeInit = bufferSize;
		int i;
		unsigned char bytes[4];
		unsigned char bytesLength = 0;
		for(i = 18; i + 6 > bufferSize; i -= 6)
			bytes[bytesLength++] = schema[(buffer >> i) & 0x3F];
		// если приём окончен, выдать дополнительные =
		if(flushed)
			switch(bufferSizeInit)
			{
			case 16: // ==
				bytes[bytesLength++] = '=';
			case 8: // =
				bytes[bytesLength++] = '=';
				break;
			}
		if(bytesLength)
			outputStream->Write(bytes, bytesLength);

		// очистить буфер
		buffer = 0;
		bufferSize = 24;
	}
}

void Base64OutputStream::ProcessDecodeBuffer()
{
	// если буфер заполнен, или объявлен конец
	if(!bufferSize || flushed)
	{
		// выдать столько целых байт, сколько есть
		// есть свободно flushed
		// 0    24
		// 6    18       не может
		// 12   12       1 байт, ==
		// 18   6        2 байта, =
		// 24   0
		int i;
		unsigned char bytes[3];
		unsigned char bytesLength = 0;
		for(i = 16; i >= bufferSize; i -= 8)
			bytes[bytesLength++] = (buffer >> i) & 0xFF;
		if(bytesLength)
			outputStream->Write(bytes, bytesLength);
		// очистить буфер
		buffer = 0;
		bufferSize = 24;
	}
}

void Base64OutputStream::Write(const void* data, size_t size)
{
	if(flushed)
		THROW("Can't write to flushed base64 output stream");

	const unsigned char* bytes = (const unsigned char*)data;
	if(encoding)
		for(size_t i = 0; i < size; ++i)
			WriteEncode(bytes[i]);
	else
		for(size_t i = 0; i < size; ++i)
			WriteDecode(bytes[i]);
}

void Base64OutputStream::Flush()
{
	if(flushed)
		return;

	flushed = true;
	// обработать последние байты
	if(encoding)
		ProcessEncodeBuffer();
	else
		ProcessDecodeBuffer();
}

ptr<File> Base64OutputStream::EncodeFile(ptr<File> file)
{
	size_t size = file->GetSize();
	ptr<MemoryStream> resultStream = NEW(MemoryStream((size + 2) / 3 * 4));
	ptr<Base64OutputStream> stream = NEW(Base64OutputStream(true, resultStream));
	stream->Write(file->GetData(), size);
	stream->Flush();
	return resultStream->ToFile();
}

END_INANITY_DATA
