#include "EmsTcpSocket.hpp"
#include "EmsService.hpp"
#include "../MemoryFile.hpp"
#include <sys/socket.h>
#include <unistd.h>

BEGIN_INANITY_NET

EmsTcpSocket::EmsTcpSocket(ptr<EmsService> service, int fd, ptr<Service::TcpSocketHandler> socketHandler)
: service(service), fd(fd), socketHandler(socketHandler)
{
	service->RegisterTcpSocket(fd, this);
}

EmsTcpSocket::~EmsTcpSocket()
{
	Close();
}

void EmsTcpSocket::Send(ptr<File> file, ptr<SendHandler> sendHandler)
{
	if(!fd) return;
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
	if(!fd) return;
	close(fd);
	service->UnregisterTcpSocket(fd);
	fd = 0;
}

void EmsTcpSocket::OnOpen()
{
	socketHandler->FireData(this);
	socketHandler = nullptr;

	// perform first read
	OnMessage();

	Dereference();
}

void EmsTcpSocket::OnMessage()
{
	if(!fd) return;
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
