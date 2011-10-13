#ifndef ___INANITY_HTTP_CLIENT_HPP___
#define ___INANITY_HTTP_CLIENT_HPP___

#include "Object.hpp"
#include "OutputStream.hpp"
#include "EventLoop.hpp"
#include "String.hpp"

BEGIN_INANITY

/// Класс HTTP-клиента.
class HttpClient : public Object
{
private:
	String request;
	ptr<OutputStream> outputStream;

	HttpClient(const String& request, ptr<OutputStream> outputStream);
	void OnConnect(ptr<ClientSocket> socket);

public:
	/// Скачать данные по HTTP.
	static void Fetch(ptr<EventLoop> eventLoop, const String& url, ptr<OutputStream> outputStream);
};

END_INANITY

#endif
