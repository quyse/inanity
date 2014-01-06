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
class Fcgi : public Object
{
private:
	class InStream;
	class OutStream;

	FCGX_Request request;

	ptr<InStream> inputStream;
	ptr<OutStream> outputStream;

public:
	Fcgi();
	~Fcgi();

	void Run(const char* socketName, int backlogSize, ptr<Handler> handler);

	char** GetEnv() const;
	const char* GetParam(const char* name) const;

	void OutputHeader(const char* header, const char* value);
	void OutputStatus(const char* status);
	void OutputContentType(const char* contentType);
	void OutputBeginResponse();

	/// Get input stream of the request.
	/** Object is the same for every request. */
	ptr<InputStream> GetInputStream() const;
	/// Get output stream of the request.
	/** Object is the same for every request. */
	ptr<OutputStream> GetOutputStream() const;
};

END_INANITY_NET

#endif
