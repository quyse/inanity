#include "EventLoop.hpp"
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "../Exception.hpp"
#include <string.h>
#include <stdio.h>

EventLoop::ConnectRequest::ConnectRequest(ptr<EventLoop> eventLoop, ptr<ConnectHandler> connectHandler)
	: eventLoop(eventLoop), connectHandler(connectHandler), stream(0)
{
}

EventLoop::EventLoop() : resolver(io_service)
{
}

EventLoop::~EventLoop()
{
}

asio::io_service& EventLoop::GetIoService()
{
	return ioService;
}

ptr<Exception> EventLoop::GetLastError()
{
	uv_err_t err = uv_last_error(loop);
	return err.code ? NEW(Exception(uv_strerror(err))) : 0;
}

ptr<ServerSocket> EventLoop::Listen(int port, ptr<ServerSocket::SocketHandler> handler)
{
	try
	{
		return NEW(ServerSocket(this,
			asio::ip::tcp::acceptor(io_service,
				asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
			handler));
	}
	catch(const asio::system_error& error)
	{
		ProcessAsioError(error);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't initialize network server", exception);
	}
}

EventLoop::ResolveRequest::ResolveRequest(ptr<EventLoop> eventLoop, ptr<ResolveResultHandler> resolveHandler)
: eventLoop(eventLoop), resolveHandler(resolveHandler)
{
}

void EventLoop::ResolveRequest::Handle(const asio::error_code& error, resolver::iterator iterator)
{
	// скопировать адреса в вектор
	std::vector<asio::ip::tcp::endpoint> endpoints;
	for(resolver::iterator end; iterator != end; ++iterator)
		endpoints.push_back(iterator);

	// вызвать обработчик
	resolveHandler->Fire(endpoints);

	// в любом случае снять лишнюю ссылку с нашего объекта
	Dereference();
}

void EventLoop::Resolve(const String& host, int port, ptr<ResolveHandler> resolveHandler)
{
	try
	{
		try
		{
			// создать запрос на разрешение имени
			
			// порт должен быть в виде строки
			char portStr[10];
			sprintf(portStr, "%d", port);

			// создать запрос
			ptr<ResolveRequest> request = NEW(ResolveRequest(this, resolveHandler);

			// запустить асинхронный запрос
			resolver.async_resolve(
				asio::ip::tcp::query(asio::ip::tcp::v4(), host, portStr),
				boost::bind(&ResolveRequest::Handle, &*request, asio::placeholders::error, asio::placeholders::iterator)
			);

			// создать дополнительную ссылку для запроса, так как он передаётся в boost::bind по указателю
			request->Reference();
		}
		catch(const asio::system_error& error)
		{
			HandleAsioError(error);
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't resolve name", exception);
	}
}

EventLoop::ConnectRequest::ConnectRequest(ptr<EventLoop> eventLoop, ptr<ConnectHandler> connectHandler)
: eventLoop(eventLoop), connectHandler(connectHandler), currentEndpointIndex(0)
{
}

void EventLoop::ConnectRequest::HandleResolveResult(const std::vector<asio::ip::tcp::endpoint>& endpoints)
{
	this->endpoints = endpoints;

	try
	{
		TryConnect();
	}
	catch(Exception* exception)
	{
		MakePointer(exception);
		connectHandler->Fire(0);
	}
}

void EventLoop::ConnectRequest::TryConnect()
{
	// если адресов не осталось
	if(currentEndpointIndex >= endpoints.size())
	{
		// вернуть ошибку
		connectHandler->Fire(0);

		return;
	}

	// попробовать очередной адрес
	if(!clientSocket)
		clientSocket = NEW(ClientSocket(eventLoop));
	socket.async_connect(endpoints[currentEndpointIndex],
		boost::bind(&ConnectRequest::HandleTryConnect, this, asio::placeholders::error));
}

void EventLoop::ConnectRequest::HandleTryConnect(const asio::error_code& error)
{
	// если получилось
	if(!error)
	{
		// вернуть результат
		connectHandler->Fire(NEW(ClientSocket(eventLoop, socket)));
		return;
	}

	// иначе попробовать следующий адрес
	socket.close();
	currentEndpointIndex++;
	TryConnect();
}

void EventLoop::Connect(const String& host, int port, ptr<ConnectHandler> handler)
{
	try
	{
		// создать запрос на соединение
		ptr<ConnectRequest> connectRequest = NEW(ConnectRequest(this, handler));

		// разрешить имя
		Resolve(host, port, ResolveHandler::Bind(connectRequest, &ConnectRequest::HandleResolveResult));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't initialize client connection", exception);
	}
}

void EventLoop::Run()
{
	ioService.run();
}

void EventLoop::HandleAsioError(const asio::system_error& error)
{
	THROW_PRIMARY_EXCEPTION(String("ASIO error: ") + error.what());
}
