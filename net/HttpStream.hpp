#ifndef ___INANITY_NET_HTTP_RESPONSE_STREAM_HPP___
#define ___INANITY_NET_HTTP_RESPONSE_STREAM_HPP___

#include "net.hpp"
#include "../deps/http-parser/http_parser.h"
#include "../Handler.hpp"
#include "../OutputStream.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY_NET

class TcpSocket;

/// Класс фильтрующего потока, выполняющего разбор HTTP-запроса/ответа, и выводящего только тело ответа.
/** Умеет также привязываться к TCP-сокету, и разбирать приходящие данные. */
class HttpStream : public OutputStream
{
public:
	typedef std::vector<std::pair<String, String> > Headers;

private:
	http_parser parser;
	static http_parser_settings settings;
	ptr<OutputStream> outputStream;
	Headers headers;
	bool lastWasHeaderField;
	bool completed;

	// обработчики событий парсера

	static int OnMessageBegin(http_parser* parser);
	static int OnUrl(http_parser* parser, const char* data, size_t size);
	static int OnStatusComplete(http_parser* parser);
	static int OnHeaderField(http_parser* parser, const char* data, size_t size);
	static int OnHeaderValue(http_parser* parser, const char* data, size_t size);
	static int OnHeadersComplete(http_parser* parser);
	static int OnBody(http_parser* parser, const char* data, size_t size);
	static int OnMessageComplete(http_parser* parser);

	// обработчики сокета
	void OnReceive(const DataHandler<ptr<File> >::Result& result);

	/// Создать поток.
	HttpStream(ptr<OutputStream> outputStream, enum http_parser_type type);

public:
	static ptr<HttpStream> CreateRequestStream(ptr<OutputStream> outputStream);
	static ptr<HttpStream> CreateResponseStream(ptr<OutputStream> outputStream);

	void Write(const void* data, size_t size);
	void End();

	/// Завершено ли корректно получение HTTP-ответа.
	bool IsCompleted() const;

	const Headers& GetHeaders() const;

	void ReceiveFromSocket(ptr<TcpSocket> socket);
};

END_INANITY_NET

#endif
