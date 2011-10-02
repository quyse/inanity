#ifndef ___INANITY_SERVER_SOCKET_HPP___
#define ___INANITY_SERVER_SOCKET_HPP___

#include "Socket.hpp"
#include "EventHandler.hpp"
#include <uv.h>

BEGIN_INANITY

class EventLoop;
class ClientSocket;

class ServerSocket : public Socket
{
public:
	typedef EventHandler<ptr<ClientSocket> > Handler;

private:
	ptr<Handler> handler;
	static void ConnectionCallback(uv_stream_t* stream, int status);

public:
	ServerSocket(ptr<EventLoop> eventLoop, uv_tcp_t* stream, ptr<Handler> handler);
};

END_INANITY

#endif
