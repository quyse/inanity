#include "Fcgi.hpp"
#include "../InputStream.hpp"
#include "../OutputStream.hpp"

BEGIN_INANITY_NET

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

class Fcgi::OutStream : public OutputStream
{
private:
	FCGX_Stream* stream;

public:
	OutStream() : stream(stream) {}

	void SetStream(FCGX_Stream* stream)
	{
		this->stream = stream;
	}

	void Write(const void* data, size_t size)
	{
		FCGX_PutStr((const char*)data, (int)size, stream);
	}
};

Fcgi::Fcgi()
{
	inputStream = NEW(InStream());
	outputStream = NEW(OutStream());
}

Fcgi::~Fcgi() {}

void Fcgi::Run(const char* socketName, int backlogSize, ptr<Handler> handler)
{
	BEGIN_TRY();

	FCGX_Init();

	int s = FCGX_OpenSocket(socketName, backlogSize);
	if(s < 0)
		THROW("Can't open socket");

	if(FCGX_InitRequest(&request, s, 0) != 0)
		THROW("Can't init request");

	while(FCGX_Accept_r(&request) >= 0)
	{
		inputStream->SetStream(request.in);
		outputStream->SetStream(request.out);
		handler->Fire();
	}

	FCGX_Free(&request, 1);

	END_TRY("Error running FCGI");
}

char** Fcgi::GetEnv() const
{
	return request.envp;
}

const char* Fcgi::GetParam(const char* name) const
{
	return FCGX_GetParam(name, request.envp);
}

ptr<InputStream> Fcgi::GetInputStream() const
{
	return inputStream;
}

ptr<OutputStream> Fcgi::GetOutputStream() const
{
	return outputStream;
}

END_INANITY_NET
