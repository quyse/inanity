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
	typedef std::function<void(ptr<Exception>)> Handler;

	/// General method for making HTTP request.
	static void Fetch(ptr<Service> service, const String& url, const String& method, ptr<File> dataFile, const std::vector<std::pair<String, String> >& headers, Handler handler, ptr<OutputStream> outputStream);
	/// Simple method for GET request.
	static void Get(ptr<Service> service, const String& url, Handler handler, ptr<OutputStream> outputStream);

private:
	class Request;
};

END_INANITY_NET

#endif
