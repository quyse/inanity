#include "HttpStream.hpp"

BEGIN_INANITY_NET

http_parser_settings HttpStream::settings = {
	HttpStream::OnMessageBegin,
	HttpStream::OnUrl,
	HttpStream::OnStatus,
	HttpStream::OnHeaderField,
	HttpStream::OnHeaderValue,
	HttpStream::OnHeadersComplete,
	HttpStream::OnBody,
	HttpStream::OnMessageComplete
};

HttpStream::HttpStream(ptr<OutputStream> outputStream, enum http_parser_type type) :
	outputStream(outputStream), lastWasHeaderField(false), completed(false)
{
	parser.data = this;
	http_parser_init(&parser, type);
}

ptr<HttpStream> HttpStream::CreateRequestStream(ptr<OutputStream> outputStream)
{
	return NEW(HttpStream(outputStream, HTTP_REQUEST));
}

ptr<HttpStream> HttpStream::CreateResponseStream(ptr<OutputStream> outputStream)
{
	return NEW(HttpStream(outputStream, HTTP_RESPONSE));
}

void HttpStream::Write(const void* data, size_t size)
{
	http_parser_execute(&parser, &settings, (const char*)data, size);
}

void HttpStream::End()
{
	http_parser_execute(&parser, &settings, 0, 0);
}

bool HttpStream::IsCompleted() const
{
	return completed;
}

const HttpStream::Headers& HttpStream::GetHeaders() const
{
	return headers;
}

int HttpStream::OnMessageBegin(http_parser* parser)
{
	return 0;
}

int HttpStream::OnUrl(http_parser* parser, const char* data, size_t size)
{
	return 0;
}

int HttpStream::OnStatus(http_parser* parser, const char* data, size_t size)
{
	return (parser->status_code >= 200 && parser->status_code <= 299) ? 0 : 1;
}

int HttpStream::OnHeaderField(http_parser* parser, const char* data, size_t size)
{
	HttpStream* stream = (HttpStream*)parser->data;

	if(stream->lastWasHeaderField)
		stream->headers.back().first += String(data, size);
	else
		stream->headers.push_back(std::make_pair(String(data, size), String()));

	return 0;
}

int HttpStream::OnHeaderValue(http_parser* parser, const char* data, size_t size)
{
	HttpStream* stream = (HttpStream*)parser->data;

	stream->headers.back().second += String(data, size);

	return 0;
}

int HttpStream::OnHeadersComplete(http_parser* parser)
{
	return 0;
}

int HttpStream::OnBody(http_parser* parser, const char* data, size_t size)
{
	HttpStream* stream = (HttpStream*)parser->data;

	stream->outputStream->Write(data, size);

	return 0;
}

int HttpStream::OnMessageComplete(http_parser* parser)
{
	HttpStream* stream = (HttpStream*)parser->data;

	stream->completed = true;

	return 0;
}

END_INANITY_NET
