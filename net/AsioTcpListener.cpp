#include "AsioTcpListener.hpp"
#include "AsioService.hpp"
#include "AsioTcpSocket.hpp"
#include <boost/bind.hpp>

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
	catch(boost::system::error_code error)
	{
	}
}

void AsioTcpListener::StartAccept()
{
	try
	{
		if(acceptingSocket)
			THROW_PRIMARY_EXCEPTION("Accepting sockets already started");

		// создать новый принимающий сокет
		acceptingSocket = NEW(AsioTcpSocket(service));
		acceptor.async_accept(acceptingSocket->GetSocket(), AcceptedBinder(this));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't start accept", exception);
	}
}

void AsioTcpListener::Accepted(const boost::system::error_code& error)
{
	if(error)
	{
		socketHandler->FireError(AsioService::ConvertError(error));
		Close();
	}
	else
	{
		socketHandler->FireData(acceptingSocket);
		acceptingSocket = 0;
		StartAccept();
	}
}
