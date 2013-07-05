#ifndef ___INANITY_NET_ASIO_UDP_LISTENER_HPP___
#define ___INANITY_NET_ASIO_UDP_LISTENER_HPP___

#include "UdpListener.hpp"
#include "asio.hpp"
#include "../CriticalSection.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_NET

class AsioInternalUdpSocket;

/// Класс UDP-слушателя на базе Asio.
class AsioUdpListener : public UdpListener
{
private:
	ptr<AsioInternalUdpSocket> internalSocket;
	/// Обработчик пакетов.
	ptr<ReceiveHandler> receiveHandler;

	CriticalSection cs;

	class Packet;

public:
	AsioUdpListener(ptr<AsioInternalUdpSocket> internalSocket, ptr<ReceiveHandler> receiveHandler);

	void Accept(const boost::asio::ip::udp::endpoint& remoteEndpoint, ptr<File> file);
	void AcceptError(const boost::system::error_code& error);

	//*** Методы UdpListener.
	void Close();
};

END_INANITY_NET

#endif
