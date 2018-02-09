#ifndef ___INANITY_NET_HTTP_CLIENT_HPP___
#define ___INANITY_NET_HTTP_CLIENT_HPP___

#include "net.hpp"
#include "../Handler.hpp"
#include "../String.hpp"

BEGIN_INANITY

class File;
class OutputStream;

END_INANITY

BEGIN_INANITY_NET

class Service;

/// HTTP client utility class.
class HttpClient : public Object
{
public:
	/// General method for making HTTP request.
	static void Fetch(ptr<Service> service, const String& url, const String& method, ptr<File> dataFile, const String& contentType, ptr<SuccessHandler> handler, ptr<OutputStream> outputStream);
	/// Simple method for GET request.
	static void Get(ptr<Service> service, const String& url, ptr<SuccessHandler> handler, ptr<OutputStream> outputStream);
};

END_INANITY_NET

#endif
