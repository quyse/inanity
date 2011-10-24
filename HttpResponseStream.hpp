#ifndef ___INANITY_HTTP_RESPONSE_STREAM_HPP___
#define ___INANITY_HTTP_RESPONSE_STREAM_HPP___

#include "OutputStream.hpp"
#include "String.hpp"
#include "http_parser.h"
#include <vector>

BEGIN_INANITY

/// Класс фильтрующего потока, выполняющего разбор HTTP-ответа, и выводящего только тело ответа.
class HttpResponseStream : public OutputStream
{
public:
	typedef std::vector<std::pair<String, String> > Headers;

private:
	http_parser parser;
	static http_parser_settings settings;
	ptr<OutputStream> outputStream;
	Headers headers;
	bool lastWasHeaderField;

	// обработчики событий парсера

	static int OnMessageBegin(http_parser* parser);
	static int OnUrl(http_parser* parser, const char* data, size_t size);
	static int OnHeaderField(http_parser* parser, const char* data, size_t size);
	static int OnHeaderValue(http_parser* parser, const char* data, size_t size);
	static int OnHeadersComplete(http_parser* parser);
	static int OnBody(http_parser* parser, const char* data, size_t size);
	static int OnMessageComplete(http_parser* parser);

public:
	/// Создать поток.
	/**
	 * \param outputStream поток, в который нужно записывать тело ответа.
	 */
	HttpResponseStream(ptr<OutputStream> outputStream);

	void Write(const void* data, size_t size);
	void Flush();

	const Headers& GetHeaders() const;
};

END_INANITY

#endif
