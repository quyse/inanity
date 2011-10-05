#include "ServerSocket.hpp"
#include "EventLoop.hpp"
#include "ClientSocket.hpp"
#include "Exception.hpp"
#include <string.h>

ServerSocket::ServerSocket(ptr<EventLoop> eventLoop, uv_tcp_t* stream, ptr<Handler> handler) :
	Socket(eventLoop, stream), handler(handler)
{
	try
	{
		if(uv_listen((uv_stream_t*)stream, 100, ConnectionCallback) != 0)
			THROW_SECONDARY_EXCEPTION("Can't listen socket", eventLoop->GetLastError());
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create server socket", exception);
	}
}

void ServerSocket::ConnectionCallback(uv_stream_t* stream, int status)
{
	if(status == 0)
	{
		uv_tcp_t* client = new uv_tcp_t;
		uv_tcp_init(stream->loop, client);
		if(uv_accept((uv_stream_t*)stream, (uv_stream_t*)client) == 0)
		{
			ServerSocket* socket = (ServerSocket*)(Socket*)stream->data;
			socket->handler->Fire(NEW(ClientSocket(socket->eventLoop, client)));
		}
	}
}
