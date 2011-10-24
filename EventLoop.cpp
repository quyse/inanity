#include "EventLoop.hpp"
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "Exception.hpp"
#include <string.h>
#include <stdio.h>

EventLoop::ConnectRequest::ConnectRequest(ptr<EventLoop> eventLoop, ptr<ConnectHandler> connectHandler)
	: eventLoop(eventLoop), connectHandler(connectHandler), stream(0)
{
}

EventLoop::EventLoop()
{
#ifdef ___INANITY_WINDOWS
	// под windows пока поддерживается только цикл по умолчанию
	loop = uv_default_loop();
#else
	loop = uv_loop_new();
#endif
	loop->data = this;
}

EventLoop::~EventLoop()
{
#ifndef ___INANITY_WINDOWS
	uv_loop_delete(loop);
#endif
}

uv_buf_t EventLoop::AllocCallback(uv_handle_t* handle, size_t size)
{
	return uv_buf_init(new char[size], size);
}

void EventLoop::Free(uv_buf_t buf)
{
	delete [] (char*)buf.base;
}

void EventLoop::GetAddrInfoCallback(uv_getaddrinfo_t* handle, int status, addrinfo* res)
{
	bool success = false;
	ConnectRequest* request = (ConnectRequest*)handle->data;

	if(status == 0)
	{
		// запустить подключение
		// пока алгоритм такой: берём первый попавшийся IPv4 или IPv6 адрес, и соединяемся с ним
		// TODO: сделать перебор всех вариантов, в случае неудачного соединения
		request->stream = new uv_tcp_t;
		uv_tcp_init(request->eventLoop->loop, request->stream);
		request->connectReq.data = request;
		for(addrinfo* addr = res; addr; addr = addr->ai_next)
		{
			int status;
			if(addr->ai_family == AF_INET)
				status = uv_tcp_connect(&request->connectReq, request->stream, *(sockaddr_in*)addr->ai_addr, ConnectCallback);
			else if(addr->ai_family == AF_INET6)
				status = uv_tcp_connect6(&request->connectReq, request->stream, *(sockaddr_in6*)addr->ai_addr, ConnectCallback);
			else
				continue;
			if(status != 0)
				continue;
			success = true;
			break;
		}

		// если неуспешно, то удалить созданный stream
		if(!success)
			delete request->stream;
	}

	// если уже известно, что соединиться не получится
	if(!success)
	{
		// сообщить обработчику об ошибке
		request->connectHandler->Fire(0);

		// удалить структуру запроса
		delete request;
	}

	// освободить память результата
	if(res)
		uv_freeaddrinfo(res);
}

void EventLoop::ConnectCallback(uv_connect_t* req, int status)
{
	ConnectRequest* request = (ConnectRequest*)req->data;

	// если соединение установлено
	if(status == 0)
		// создать сокет и передать его обработчику
		// при этом stream переходит к сокету, и он уже за него отвечает
		request->connectHandler->Fire(NEW(ClientSocket(request->eventLoop, request->stream)));
	else
	{
		// ошибка, сообщить обработчику
		request->connectHandler->Fire(0);
		// удалить stream
		delete request->stream;
	}

	// и в любом случае удалить структуру запроса
	delete request;
}

ptr<Exception> EventLoop::GetLastError()
{
	uv_err_t err = uv_last_error(loop);
	return err.code ? NEW(Exception(uv_strerror(err))) : 0;
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
			THROW_SECONDARY_EXCEPTION("Can't bind socket", GetLastError());
		}

		return NEW(ServerSocket(this, stream, handler));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't initialize network server", exception);
	}
}

void EventLoop::Connect(const String& host, int port, ptr<ConnectHandler> handler)
{
	try
	{
		ConnectRequest* request = new ConnectRequest(this, handler);
		request->getaddrinfoReq.data = request;

		// сформировать структуру "подсказок"
		// пока поддерживаем только IPv4
		addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_NUMERICSERV;

		char portStr[10];
		sprintf(portStr, "%d", port);

		if(uv_getaddrinfo(loop, &request->getaddrinfoReq, GetAddrInfoCallback, host.c_str(), portStr, &hints) != 0)
		{
			delete request;
			THROW_SECONDARY_EXCEPTION("Can't get addr info", GetLastError());
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't initialize client connection", exception);
	}
}

void EventLoop::Run()
{
	uv_run(loop);
}
