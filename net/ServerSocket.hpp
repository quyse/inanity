#ifndef ___INANITY_NET_SERVER_SOCKET_HPP___
#define ___INANITY_NET_SERVER_SOCKET_HPP___

#include "Socket.hpp"
#include "../Handler.hpp"

BEGIN_INANITY_NET

class EventLoop;
class ClientSocket;

class ServerSocket : public Socket
{
public:
	typedef Handler<ptr<ClientSocket> > SocketHandler;

private:
	ptr<SocketHandler> handler;
	static void ConnectionCallback(uv_stream_t* stream, int status);

public:
	ServerSocket(ptr<EventLoop> eventLoop, uv_tcp_t* stream, ptr<SocketHandler> handler);
};

END_INANITY_NET

#endif
