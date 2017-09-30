#include "EmsTcpSocket.hpp"
#include "EmsService.hpp"
#include "../MemoryFile.hpp"
#include <sys/socket.h>

BEGIN_INANITY_NET

EmsTcpSocket::EmsTcpSocket(ptr<EmsService> service, int fd, ptr<Service::TcpSocketHandler> socketHandler)
: service(service), fd(fd), socketHandler(socketHandler)
{
	service->RegisterTcpSocket(fd, this);
}

EmsTcpSocket::~EmsTcpSocket()
{
	service->UnregisterTcpSocket(fd);
}

void EmsTcpSocket::Send(ptr<File> file, ptr<SendHandler> sendHandler)
{
	send(fd, file->GetData(), file->GetSize(), 0);
	if(sendHandler)
		sendHandler->FireSuccess();
}

void EmsTcpSocket::End()
{
}

void EmsTcpSocket::SetReceiveHandler(ptr<ReceiveHandler> receiveHandler)
{
	this->receiveHandler = receiveHandler;
}

void EmsTcpSocket::SetNoDelay(bool noDelay)
{
}

void EmsTcpSocket::Close()
{
}

void EmsTcpSocket::OnOpen()
{
	socketHandler->FireData(this);
	socketHandler = nullptr;
	Dereference();
}

void EmsTcpSocket::OnMessage()
{
	uint8_t buf[4096];
	ssize_t len;
	while((len = recv(fd, buf, sizeof(buf), 0)) > 0)
	{
		if(receiveHandler)
			receiveHandler->FireData(MemoryFile::CreateViaCopy(buf, len));
	}
}

void EmsTcpSocket::OnClose()
{
}

END_INANITY_NET
