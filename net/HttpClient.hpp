#ifndef ___INANITY_NET_HTTP_CLIENT_HPP___
#define ___INANITY_NET_HTTP_CLIENT_HPP___

#include "net.hpp"
#include "../Handler.hpp"
#include "../String.hpp"

BEGIN_INANITY

class OutputStream;

END_INANITY

BEGIN_INANITY_NET

class Service;

/// HTTP client utility class.
class HttpClient : public Object
{
public:
	/// Download data via HTTP.
	static void Fetch(ptr<Service> service, const String& url, ptr<SuccessHandler> handler, ptr<OutputStream> outputStream);
};

END_INANITY_NET

#endif
