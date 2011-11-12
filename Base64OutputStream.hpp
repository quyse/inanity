#ifndef ___INANITY_BASE64_OUTPUT_STREAM_HPP___
#define ___INANITY_BASE64_OUTPUT_STREAM_HPP___

#include "OutputStream.hpp"

BEGIN_INANITY

/// Поток, кодирующий/декодирующий поступающие данные в Base64.
class Base64OutputStream : public OutputStream
{
private:
	static const char schema[65];
	static unsigned char inverseSchema[256];

	bool encoding;
	ptr<OutputStream> outputStream;
	/// 24-битный буфер данных
	unsigned buffer;
	/// сколько ещё в буфере места, в битах
	int bufferSize;
	/// завершение кодирования
	bool flushed;

private:
	void WriteEncode(unsigned char byte);
	void ProcessEncodeBuffer();
	void WriteDecode(unsigned char byte);
	void ProcessDecodeBuffer();

public:
	Base64OutputStream(bool encoding, ptr<OutputStream> outputStream);

	void Write(const void* data, size_t size);
	void Flush();
};

END_INANITY

#endif
