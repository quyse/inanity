#ifndef ___INANITY_NET_FCGI_HPP___
#define ___INANITY_NET_FCGI_HPP___

#include "net.hpp"
#include "../Handler.hpp"
#include "../deps/fcgi/fcgiapp.h"

BEGIN_INANITY

class InputStream;
class OutputStream;

END_INANITY

BEGIN_INANITY_NET

/// Simple class for single-threaded FCGI handler.
class Fcgi
{
private:
	class InStream;
	class OutStream;

	int fd;

public:
	class Request : public Object
	{
	private:
		FCGX_Request request;
		ptr<InStream> inputStream;
		ptr<OutStream> outputStream;

	public:
		Request(int fd);
		~Request();

		ptr<InputStream> GetInputStream() const;
		ptr<OutputStream> GetOutputStream() const;

		char** GetEnv() const;
		const char* GetParam(const char* name) const;

		void OutputHeader(const char* header, const char* value);
		void OutputStatus(const char* status);
		void OutputContentType(const char* contentType);
		void OutputBeginResponse();
		/// End response and finalize request.
		void End();

		//*** Don't call explicitly.
		bool Accept();
	};

public:
	Fcgi(const char* socketName, int backlogSize);

	ptr<Request> Accept();
};

END_INANITY_NET

#endif
