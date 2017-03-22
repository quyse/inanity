#include "AsioTcpListener.hpp"
#include "AsioService.hpp"
#include "AsioTcpSocket.hpp"
#include <boost/bind.hpp>

BEGIN_INANITY_NET

/// Вспомогательный класс для обработки входящего сообщения.
class AsioTcpListener::AcceptedBinder
{
private:
	ptr<AsioTcpListener> listener;

public:
	AcceptedBinder(ptr<AsioTcpListener> listener) : listener(listener) {}

	void operator()(const boost::system::error_code& error) const
	{
		listener->Accepted(error);
	}
};

AsioTcpListener::AsioTcpListener(ptr<AsioService> service, int port, ptr<SocketHandler> socketHandler)
: service(service), socketHandler(socketHandler),
	acceptor(
		service->GetIoService(),
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)
	)
{
	acceptor.listen();
	StartAccept();
}

void AsioTcpListener::Close()
{
	try
	{
		acceptor.close();
	}
	catch(boost::system::system_error error)
	{
	}

	socketHandler = nullptr;
}

void AsioTcpListener::StartAccept()
{
	try
	{
		if(acceptingSocket)
			THROW("Accepting sockets already started");

		// создать новый принимающий сокет
		acceptingSocket = NEW(AsioTcpSocket(service));
		acceptor.async_accept(acceptingSocket->GetSocket(), AcceptedBinder(this));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't start accept", exception);
	}
}

void AsioTcpListener::Accepted(const boost::system::error_code& error)
{
	if(error)
	{
		if(socketHandler)
			socketHandler->FireError(AsioService::ConvertError(error));
		Close();
	}
	else
	{
		if(socketHandler)
			socketHandler->FireData(acceptingSocket);
		acceptingSocket = nullptr;
		StartAccept();
	}
}

END_INANITY_NET
