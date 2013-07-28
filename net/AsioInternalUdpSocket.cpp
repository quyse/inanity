#include "AsioInternalUdpSocket.hpp"
#include "AsioService.hpp"
#include "AsioUdpListener.hpp"
#include "AsioUdpSocket.hpp"
#include "../MemoryFile.hpp"
#include "../PartFile.hpp"
#include "../CriticalCode.hpp"
#include <vector>

BEGIN_INANITY_NET

/* Policy для многопоточности.
private-функции не делают синхронизации, и её нужно делать самим при вызове.
public-функции делают синхронизацию.
*/

class AsioInternalUdpSocket::ReceiveBinder
{
private:
	ptr<AsioInternalUdpSocket> socket;

public:
	ReceiveBinder(ptr<AsioInternalUdpSocket> socket) : socket(socket) {}

	void operator()(const boost::system::error_code& error, size_t transferred) const
	{
		socket->Received(error, transferred);
	}
};

const size_t AsioInternalUdpSocket::receiveFileSize = 0x1000;

AsioInternalUdpSocket::AsioInternalUdpSocket(ptr<AsioService> service)
: service(service), socket(service->GetIoService()) {}

boost::asio::ip::udp::socket& AsioInternalUdpSocket::GetSocket()
{
	return socket;
}

CriticalSection& AsioInternalUdpSocket::GetCriticalSection()
{
	return cs;
}

void AsioInternalUdpSocket::Start()
{
	CriticalCode cc(cs);

	StartReceive();
}

void AsioInternalUdpSocket::SetClientListener(ptr<AsioUdpListener> clientListener)
{
	CriticalCode cc(cs);

	this->clientListener = clientListener;
}

void AsioInternalUdpSocket::RegisterClientSocket(const boost::asio::ip::udp::endpoint& endpoint, ptr<AsioUdpSocket> clientSocket)
{
	CriticalCode cc(cs);

	if(clientSockets.find(endpoint) != clientSockets.end())
		THROW("This endpoint already registered");

	clientSockets[endpoint] = clientSocket;
}

void AsioInternalUdpSocket::UnregisterClientSocket(const boost::asio::ip::udp::endpoint& endpoint)
{
	bool noClientSockets;
	{
		CriticalCode cc(cs);

		ClientSockets::iterator i = clientSockets.find(endpoint);
		if(i == clientSockets.end())
			THROW("This endpoint is not registered");

		clientSockets.erase(i);

		noClientSockets = clientSockets.empty();
	}

	if(noClientSockets)
		Close();
}

void AsioInternalUdpSocket::Close()
{
	CriticalCode cc(cs);

	try
	{
		socket.close();
	}
	catch(boost::system::system_error error)
	{
	}
}

void AsioInternalUdpSocket::StartReceive()
{
	try
	{
		if(receiveFile)
			THROW("Receive already started");

		receiveFile = NEW(MemoryFile(receiveFileSize));

		try
		{
			socket.async_receive_from(
				boost::asio::buffer(receiveFile->GetData(), receiveFileSize),
				receiveEndpoint,
				ReceiveBinder(this)
			);
		}
		catch(boost::system::system_error error)
		{
			THROW_SECONDARY("Asio error", AsioService::ConvertError(error));
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't start receiving Asio UDP socket", exception);
	}
}

void AsioInternalUdpSocket::Received(const boost::system::error_code& error, size_t transferred)
{
	if(error)
	{
		// сообщить всем, что произошла ошибка
		ptr<AsioUdpListener> clientListener;
		std::vector<ptr<AsioUdpSocket> > clientSockets;
		// скопировать получателей в локальные переменные, чтобы не получить
		// взаимную блокировку, когда будем передавать ошибку
		{
			CriticalCode cc(cs);

			clientListener = this->clientListener;
			clientSockets.reserve(this->clientSockets.size());
			for(ClientSockets::const_iterator i = this->clientSockets.begin(); i != this->clientSockets.end(); ++i)
				clientSockets.push_back(i->second);
		}
		if(clientListener)
			clientListener->AcceptError(error);
		for(size_t i = 0; i < clientSockets.size(); ++i)
			clientSockets[i]->ReceiveError(error);

		// закрыть сокет
		Close();
	}
	else
	{
		ptr<AsioUdpListener> clientListener;
		ptr<AsioUdpSocket> clientSocket;
		ptr<File> packetFile;

		{
			CriticalCode cc(cs);
			// прежде всего, возобновить приём пакетов
			packetFile = receiveFile;
			receiveFile = 0;
			StartReceive();

			// найти сокет, в который должен поступить пакет
			ClientSockets::iterator i = clientSockets.find(receiveEndpoint);
			if(i == clientSockets.end())
				clientListener = this->clientListener;
			else
				clientSocket = i->second;

			// блокировка заканчивается здесь
		}

		// если сокет нашёлся, передать ему пакет
		if(clientSocket)
			clientSocket->Receive(NEW(PartFile(packetFile, packetFile->GetData(), transferred)));
		// иначе, если слушатель есть, передать ему пакет
		else if(clientListener)
			clientListener->Accept(receiveEndpoint, NEW(PartFile(packetFile, packetFile->GetData(), transferred)));
	}
}

END_INANITY_NET
