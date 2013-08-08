#ifndef ___INANITY_NET_ASIO_UDP_SOCKET_HPP___
#define ___INANITY_NET_ASIO_UDP_SOCKET_HPP___

#include "UdpSocket.hpp"
#include "asio.hpp"
#include "../CriticalSection.hpp"

BEGIN_INANITY_NET

class AsioInternalUdpSocket;

class AsioUdpSocket : public UdpSocket
{
private:
	ptr<AsioInternalUdpSocket> internalSocket;
	ptr<ReceiveHandler> receiveHandler;
	boost::asio::ip::udp::endpoint remoteEndpoint;

	CriticalSection cs;

	class Buffers1;
	class SentBinder;

	void Sent(const boost::system::error_code& error, size_t transferred);

public:
	AsioUdpSocket(ptr<AsioInternalUdpSocket> internalSocket, const boost::asio::ip::udp::endpoint& remoteEndpoint);

	void Receive(ptr<File> file);
	void ReceiveError(const boost::system::error_code& error);

	//*** методы UdpSocket
	void Send(ptr<File> file);
	void SetReceiveHandler(ptr<ReceiveHandler> receiveHandler);
	void Close();
};

END_INANITY_NET

#endif
