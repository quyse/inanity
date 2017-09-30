#ifndef ___INANITY_NET_EMS_TCP_SOCKET_HPP___
#define ___INANITY_NET_EMS_TCP_SOCKET_HPP___

#include "TcpSocket.hpp"
#include "Service.hpp"

BEGIN_INANITY_NET

class EmsService;

/// TCP socket (actually websocket) for emscripten network service.
class EmsTcpSocket : public TcpSocket
{
public:
	EmsTcpSocket(ptr<EmsService> service, int fd, ptr<Service::TcpSocketHandler> socketHandler);
	~EmsTcpSocket();

	//*** TcpSocket's methods
	void Send(ptr<File> file, ptr<SendHandler> sendHandler) override;
	void End() override;
	void SetReceiveHandler(ptr<ReceiveHandler> receiveHandler) override;
	void SetNoDelay(bool noDelay) override;
	void Close() override;

	void OnOpen();
	void OnMessage();
	void OnClose();

private:
	ptr<EmsService> service;
	int fd;
	ptr<Service::TcpSocketHandler> socketHandler;
	ptr<ReceiveHandler> receiveHandler;
};

END_INANITY_NET

#endif
