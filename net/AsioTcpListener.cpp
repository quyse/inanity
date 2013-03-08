#include "AsioTcpListener.hpp"
#include "AsioService.hpp"
#include "AsioTcpSocket.hpp"
#include <boost/bind.hpp>

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

void AsioTcpListener::StartAccept()
{
	try
	{
		if(acceptingSocket)
			THROW_PRIMARY_EXCEPTION("Accepting sockets already started");

		acceptingSocket = NEW(AsioTcpSocket(service));
		acceptor.async_accept(acceptingSocket->GetSocket(), boost::bind(&AsioTcpListener::Accepted, this, boost::asio::placeholders::error));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't start accept", exception);
	}
}

void AsioTcpListener::Accepted(const boost::system::error_code& error)
{
	if(error)
		socketHandler->FireError(AsioService::ConvertError(error));
	else
	{
		socketHandler->FireData(acceptingSocket);
		acceptingSocket = 0;
		StartAccept();
	}
}
