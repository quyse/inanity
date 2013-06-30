#include "HttpResponseStream.hpp"

BEGIN_INANITY

http_parser_settings HttpResponseStream::settings = {
	HttpResponseStream::OnMessageBegin,
	HttpResponseStream::OnUrl,
	HttpResponseStream::OnHeaderField,
	HttpResponseStream::OnHeaderValue,
	HttpResponseStream::OnHeadersComplete,
	HttpResponseStream::OnBody,
	HttpResponseStream::OnMessageComplete
};

HttpResponseStream::HttpResponseStream(ptr<OutputStream> outputStream) :
	outputStream(outputStream), lastWasHeaderField(false)
{
	parser.data = this;
	http_parser_init(&parser, HTTP_RESPONSE);
}

void HttpResponseStream::Write(const void* data, size_t size)
{
	http_parser_execute(&parser, &settings, (const char*)data, size);
}

void HttpResponseStream::Flush()
{
	http_parser_execute(&parser, &settings, 0, 0);
}

const HttpResponseStream::Headers& HttpResponseStream::GetHeaders() const
{
	return headers;
}

int HttpResponseStream::OnMessageBegin(http_parser* parser)
{
	return 0;
}

int HttpResponseStream::OnUrl(http_parser* parser, const char* data, size_t size)
{
	return 0;
}

int HttpResponseStream::OnHeaderField(http_parser* parser, const char* data, size_t size)
{
	HttpResponseStream* stream = (HttpResponseStream*)parser->data;

	if(stream->lastWasHeaderField)
		stream->headers.back().first += String(data, size);
	else
		stream->headers.push_back(std::make_pair(String(data, size), String()));

	return 0;
}

int HttpResponseStream::OnHeaderValue(http_parser* parser, const char* data, size_t size)
{
	HttpResponseStream* stream = (HttpResponseStream*)parser->data;

	stream->headers.back().second += String(data, size);

	return 0;
}

int HttpResponseStream::OnHeadersComplete(http_parser* parser)
{
	return 0;
}

int HttpResponseStream::OnBody(http_parser* parser, const char* data, size_t size)
{
	HttpResponseStream* stream = (HttpResponseStream*)parser->data;

	stream->outputStream->Write(data, size);

	return 0;
}

int HttpResponseStream::OnMessageComplete(http_parser* parser)
{
	HttpResponseStream* stream = (HttpResponseStream*)parser->data;

	stream->outputStream->Flush();

	return 0;
}

END_INANITY
