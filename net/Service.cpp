#include "Service.hpp"

BEGIN_INANITY_NET

ptr<TcpListener> Service::ListenTcp(int port, ptr<TcpSocketHandler> socketHandler)
{
	THROW("Listen TCP not implemented");
}

void Service::ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler)
{
	THROW("Connect TCP not implemented");
}

ptr<UdpListener> Service::ListenUdp(int port, ptr<UdpPacketHandler> receiveHandler)
{
	THROW("Listen UDP not implemented");
}

void Service::ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> socketHandler)
{
	THROW("Connect UDP not implemented");
}

void Service::ConnectTlsTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler)
{
	THROW("Connect TCP with TLS not implemented");
}

END_INANITY_NET
