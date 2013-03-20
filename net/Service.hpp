#ifndef ___INANITY_NET_SERVICE_HPP___
#define ___INANITY_NET_SERVICE_HPP___

#include "net.hpp"
#include "../Handler.hpp"
#include "../String.hpp"

BEGIN_INANITY_NET

class TcpListener;
class TcpSocket;
class UdpListener;
class UdpSocket;
class UdpPacket;

/// Абстрактный класс сетевой службы.
class Service : public Object
{
public:
	typedef DataHandler<ptr<TcpSocket> > TcpSocketHandler;
	typedef DataHandler<ptr<UdpPacket> > UdpPacketHandler;
	typedef DataHandler<ptr<UdpSocket> > UdpSocketHandler;

public:
	/// Выполнять работу.
	virtual void Run() = 0;
	/// Остановить работу.
	virtual void Stop() = 0;

	/// Открыть порт и начать ожидать входящие TCP соединения.
	/** Один раз будет вызван listenerHandler, и затем вызывается socketHandler
	по разу на каждое входящее соединение, пока существует объект TcpListener. */
	virtual ptr<TcpListener> ListenTcp(int port, ptr<TcpSocketHandler> socketHandler) = 0;
	/// Создать исходящее TCP-подключение.
	/** socketHandler будет вызван один раз. */
	virtual void ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler) = 0;

	/// Открыть порт и начать ожидать UDP-пакеты.
	virtual ptr<UdpListener> ListenUdp(int port, ptr<UdpPacketHandler> receiveHandler) = 0;
	/// Создать исходящее UDP-подключение.
	virtual void ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> socketHandler) = 0;
};

END_INANITY_NET

#endif
