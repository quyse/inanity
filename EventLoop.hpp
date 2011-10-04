#ifndef ___INANITY_EVENT_LOOP_HPP___
#define ___INANITY_EVENT_LOOP_HPP___

#include "Object.hpp"
#include "EventHandler.hpp"
#include "ServerSocket.hpp"
#include "String.hpp"
#include <uv.h>

/*
 * Файл содержит класс цикла обработки асинхронных событий,
 * основанного на библиотеке libuv.
 */

BEGIN_INANITY

class ClientSocket;

class EventLoop : public Object
{
	friend class ServerSocket;
public:
	typedef EventHandler<ptr<ClientSocket> > ConnectHandler;

private:
	uv_loop_t* loop;

	/// Структура, содержащая всё необходимое для выполнения запроса на соединение.
	struct ConnectRequest
	{
		ptr<EventLoop> eventLoop;
		ptr<ConnectHandler> connectHandler;
		uv_getaddrinfo_t getaddrinfoReq;
		uv_tcp_t* stream;
		uv_connect_t connectReq;

		ConnectRequest(ptr<EventLoop> eventLoop, ptr<ConnectHandler> connectHandler);
	};

public:
	EventLoop();
	~EventLoop();

	static uv_buf_t AllocCallback(uv_handle_t* handle, size_t size);
	static void Free(uv_buf_t buf);
	static void GetAddrInfoCallback(uv_getaddrinfo_t* handle, int status, struct addrinfo* res);
	static void ConnectCallback(uv_connect_t* req, int status);

	ptr<ServerSocket> Listen(int port, ptr<ServerSocket::Handler> handler);
	void Connect(const String& host, int port, ptr<ConnectHandler> handler);

	void Run();
};

END_INANITY

#endif
