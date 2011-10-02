#include "EventLoop.hpp"
#include "ServerSocket.cpp"

EventLoop::EventLoop()
{
	loop = uv_loop_new();
}

EventLoop::~EventLoop()
{
	uv_loop_delete(loop);
}

uv_buf_t EventLoop::AllocCallback(uv_handle_t* handle, size_t size)
{
	return uv_buf_init(new char[size], size);
}

void EventLoop::Free(uv_buf_t buf)
{
	delete [] (char*)buf.base;
}

ptr<ServerSocket> EventLoop::Listen(int port, ptr<ServerSocket::Handler> handler)
{
	try
	{
		uv_tcp_t* stream = new uv_tcp_t;
		uv_tcp_init(loop, stream);
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);
		if(uv_tcp_bind(stream, addr) != 0)
		{
			delete stream;
			THROW_PRIMARY_EXCEPTION("Can't bind socket");
		}

		return NEW(ServerSocket(this, stream, handler));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create server socket", exception);
	}
}
