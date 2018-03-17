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

/// Network service abstract class.
class Service : public Object
{
public:
	typedef DataHandler<ptr<TcpSocket> > TcpSocketHandler;
	typedef DataHandler<ptr<UdpPacket> > UdpPacketHandler;
	typedef DataHandler<ptr<UdpSocket> > UdpSocketHandler;

public:
	/// Do network job.
	/** Doesn't return control until stopped. */
	virtual void Run() = 0;
	/// Stop network job.
	/** Could be called from any thread. Causes Run() to return control in threads
	which called Run(). */
	virtual void Stop() = 0;

	/// Wait asyncronously for specified number of seconds.
	virtual void Wait(std::function<void()> handler, int seconds) = 0;

	/// Open a TCP port and begin listening incoming TCP connections.
	virtual ptr<TcpListener> ListenTcp(int port, ptr<TcpSocketHandler> socketHandler);
	/// Create outcoming TCP connection.
	/** socketHandler will be called once. */
	virtual void ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler);

	/// Open an UDP port and begin listening incoming UDP packets.
	virtual ptr<UdpListener> ListenUdp(int port, ptr<UdpPacketHandler> receiveHandler);
	/// Create outcoming UDP connection.
	/** Of course this is not actually connection, but only an abstraction. */
	virtual void ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> socketHandler);
};

END_INANITY_NET

#endif
