#include "AsioUdpListener.hpp"
#include "AsioInternalUdpSocket.hpp"
#include "AsioService.hpp"
#include "AsioUdpSocket.hpp"
#include "UdpPacket.hpp"
#include "../MemoryFile.hpp"
#include "../PartFile.hpp"
#include "../CriticalCode.hpp"

BEGIN_INANITY_NET

class AsioUdpListener::Packet : public UdpPacket
{
private:
	ptr<AsioUdpListener> listener;
	ptr<File> file;
	boost::asio::ip::udp::endpoint remoteEndpoint;

public:
	Packet(ptr<AsioUdpListener> listener, ptr<File> file, const boost::asio::ip::udp::endpoint& remoteEndpoint)
	: listener(listener), file(file), remoteEndpoint(remoteEndpoint) {}

	//*** Методы UdpListener::Packet.
	ptr<File> GetData() const
	{
		return file;
	}

	ptr<UdpSocket> CreateSocket()
	{
		try
		{
			CriticalCode cc(listener->cs);
			return NEW(AsioUdpSocket(listener->internalSocket, remoteEndpoint));
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Can't create Asio UDP socket for packet", exception);
		}
	}
};

AsioUdpListener::AsioUdpListener(ptr<AsioInternalUdpSocket> internalSocket, ptr<ReceiveHandler> receiveHandler)
: internalSocket(internalSocket), receiveHandler(receiveHandler)
{
	internalSocket->SetClientListener(this);
}

void AsioUdpListener::Accept(const boost::asio::ip::udp::endpoint& remoteEndpoint, ptr<File> file)
{
	ptr<ReceiveHandler> receiveHandler;
	{
		CriticalCode cc(cs);
		receiveHandler = this->receiveHandler;
	}

	if(receiveHandler)
		receiveHandler->FireData(NEW(Packet(this, file, remoteEndpoint)));
}

void AsioUdpListener::AcceptError(const boost::system::error_code& error)
{
	ptr<ReceiveHandler> receiveHandler;

	{
		CriticalCode cc(cs);
		receiveHandler = this->receiveHandler;
	}

	if(receiveHandler)
		receiveHandler->FireError(AsioService::ConvertError(error));

	Close();
}

void AsioUdpListener::Close()
{
	CriticalCode cc(cs);

	if(internalSocket)
	{
		internalSocket->SetClientListener(0);
		internalSocket = 0;
	}
}

END_INANITY_NET
