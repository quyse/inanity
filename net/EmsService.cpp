#include "EmsService.hpp"
#include "EmsTcpSocket.hpp"
#include <emscripten.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <assert.h>

BEGIN_INANITY_NET

EmsService::EmsService() {}

void EmsService::Run()
{
	emscripten_set_socket_open_callback(this, &EmsService::StaticOpenCallback);
	emscripten_set_socket_message_callback(this, &EmsService::StaticMessageCallback);
	emscripten_set_socket_close_callback(this, &EmsService::StaticCloseCallback);
}

void EmsService::Stop()
{
	emscripten_set_socket_connection_callback(nullptr, nullptr);
	emscripten_set_socket_message_callback(nullptr, nullptr);
	emscripten_set_socket_close_callback(nullptr, nullptr);
}

void EmsService::Wait(std::function<void()> handler, int seconds)
{
	emscripten_async_call([](void* vhandlerCopy)
	{
		std::function<void()>* handlerCopy = static_cast<std::function<void()>*>(vhandlerCopy);
		(*handlerCopy)();
		delete handlerCopy;
	}, new std::function<void()>(handler), seconds * 1000);
}

void EmsService::ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler)
{
	BEGIN_TRY();

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	char service[16];
	sprintf(service, "%d", port);
	struct addrinfo* servinfo;
	if(getaddrinfo(host.c_str(), service, &hints, &servinfo))
		THROW("Can't get addr info");

	for(struct addrinfo* r = servinfo; r; r = r -> ai_next)
	{
		int fd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
		if(fd < 0)
			continue;

		ptr<EmsTcpSocket> socket = NEW(EmsTcpSocket(this, fd, socketHandler));

		fcntl(fd, F_SETFL, O_NONBLOCK);

		if(connect(fd, r->ai_addr, r->ai_addrlen) && errno != EINPROGRESS)
			THROW("Can't connect");

		socket->Reference();

		return;
	}

	END_TRY("Can't connect Ems TCP socket");
}

void EmsService::RegisterTcpSocket(int fd, EmsTcpSocket* tcpSocket)
{
	assert(!tcpSockets.count(fd));
	tcpSockets[fd] = tcpSocket;
}

void EmsService::UnregisterTcpSocket(int fd)
{
	tcpSockets.erase(fd);
}

void EmsService::OpenCallback(int fd)
{
	auto i = tcpSockets.find(fd);
	if(i != tcpSockets.end())
		i->second->OnOpen();
}

void EmsService::MessageCallback(int fd)
{
	auto i = tcpSockets.find(fd);
	if(i != tcpSockets.end())
		i->second->OnMessage();
}

void EmsService::CloseCallback(int fd)
{
	auto i = tcpSockets.find(fd);
	if(i != tcpSockets.end())
		i->second->OnClose();
}

void EmsService::StaticOpenCallback(int fd, void* userData)
{
	((EmsService*)userData)->OpenCallback(fd);
}

void EmsService::StaticMessageCallback(int fd, void* userData)
{
	((EmsService*)userData)->MessageCallback(fd);
}

void EmsService::StaticCloseCallback(int fd, void* userData)
{
	((EmsService*)userData)->CloseCallback(fd);
}

END_INANITY_NET
