#ifndef ___INANITY_NET_SERVICE_HPP___
#define ___INANITY_NET_SERVICE_HPP___

#include "net.hpp"
#include "../Handler.hpp"
#include "../String.hpp"

BEGIN_INANITY_NET

class TcpListener;
class TcpSocket;
class UdpSocket;
class UdpPacket;

/// Абстрактный класс сетевой службы.
class Service : public Object
{
public:
	typedef DataHandler<ptr<TcpSocket> > TcpSocketHandler;
	typedef DataHandler<ptr<UdpSocket> > UdpSocketHandler;
	typedef DataHandler<ptr<UdpPacket> > UdpPacketHandler;

public:
	/// Выполнять работу.
	virtual void Run() = 0;

	/// Открыть порт и начать ожидать входящие TCP соединения.
	/** Один раз будет вызван listenerHandler, и затем вызывается socketHandler
	по разу на каждое входящее соединение, пока существует объект TcpListener. */
	virtual ptr<TcpListener> ListenTcp(int port, ptr<TcpSocketHandler> socketHandler) = 0;
	/// Создать исходящее TCP-подключение.
	/** socketHandler будет вызван один раз. */
	virtual void ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler) = 0;

	/// Открыть порт и начать ожидать UDP-пакеты.
	/** Один раз будет вызван listenerHandler, и затем вызывается packetHandler
	по разу на каждый входящий UDP-пакет. */
	virtual void ListenUdp(int port, ptr<UdpSocketHandler> socketHandler, ptr<UdpPacketHandler> packetHandler) = 0;
	/// Создать исходящее UDP-подключение.
	virtual void ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> connectHandler, ptr<UdpPacketHandler> packetHandler) = 0;
};

END_INANITY_NET

#endif
