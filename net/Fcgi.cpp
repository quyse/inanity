#include "Fcgi.hpp"
#include "../InputStream.hpp"
#include "../OutputStream.hpp"

BEGIN_INANITY_NET

// class Fcgi::InStream

class Fcgi::InStream : public InputStream
{
private:
	FCGX_Stream* stream;

public:
	InStream() : stream(nullptr) {}

	void SetStream(FCGX_Stream* stream)
	{
		this->stream = stream;
	}

	size_t Read(void* data, size_t size)
	{
		return FCGX_GetStr((char*)data, (int)size, stream);
	}
};

// class Fcgi::OutStream

class Fcgi::OutStream : public OutputStream
{
private:
	FCGX_Stream* stream;

public:
	OutStream() : stream(nullptr) {}

	void SetStream(FCGX_Stream* stream)
	{
		this->stream = stream;
	}

	void Write(const void* data, size_t size)
	{
		if(FCGX_PutStr((const char*)data, (int)size, stream) != (int)size)
			THROW("Can't write to FCGI stream");
	}
};

// class Fcgi::Request

Fcgi::Request::Request(int fd)
{
	if(FCGX_InitRequest(&request, fd, 0) != 0)
		THROW("Can't init request");

	inputStream = NEW(InStream());
	outputStream = NEW(OutStream());
}

Fcgi::Request::~Request()
{
	FCGX_Free(&request, 1);
}

ptr<InputStream> Fcgi::Request::GetInputStream() const
{
	return inputStream;
}

ptr<OutputStream> Fcgi::Request::GetOutputStream() const
{
	return outputStream;
}

char** Fcgi::Request::GetEnv() const
{
	return request.envp;
}

const char* Fcgi::Request::GetParam(const char* name) const
{
	return FCGX_GetParam(name, request.envp);
}

void Fcgi::Request::OutputHeader(const char* header, const char* value)
{
	FCGX_FPrintF(request.out, "%s: %s\r\n", header, value);
}

void Fcgi::Request::OutputStatus(const char* status)
{
	OutputHeader("Status", status);
}

void Fcgi::Request::OutputContentType(const char* contentType)
{
	OutputHeader("ContentType", contentType);
}

void Fcgi::Request::OutputBeginResponse()
{
	FCGX_PutS("\r\n", request.out);
}

void Fcgi::Request::End()
{
	FCGX_Finish_r(&request);
}

bool Fcgi::Request::Accept()
{
	if(FCGX_Accept_r(&request) < 0)
		return false;

	inputStream->SetStream(request.in);
	outputStream->SetStream(request.out);

	return true;
}

// class Fcgi

Fcgi::Fcgi(const char* socketName, int backlogSize)
{
	FCGX_Init();

	fd = FCGX_OpenSocket(socketName, backlogSize);
	if(fd < 0)
		THROW("Can't open socket");
}

ptr<Fcgi::Request> Fcgi::Accept()
{
	ptr<Request> request = NEW(Request(fd));

	if(!request->Accept())
		return nullptr;

	return request;
}

END_INANITY_NET
