#include "AsioTcpSocket.hpp"
#include "AsioBaseTcpSocket.ipp"

BEGIN_INANITY_NET

template class AsioBaseTcpSocket<boost::asio::ip::tcp::socket>;

AsioTcpSocket::AsioTcpSocket(ptr<AsioService> service)
: AsioBaseTcpSocket(service, service->GetIoService()) {}

void AsioTcpSocket::ShutdownNonSynced()
{
	try
	{
		socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
	}
	catch(boost::system::system_error)
	{
	}
}

void AsioTcpSocket::CloseNonSynced()
{
	try
	{
		socket.close();
	}
	catch(boost::system::system_error error)
	{
	}
}

boost::asio::ip::tcp::socket& AsioTcpSocket::GetLowestSocket()
{
	return socket;
}

void AsioTcpSocket::SetNoDelay(bool noDelay)
{
	socket.set_option(boost::asio::ip::tcp::no_delay(noDelay));
}

END_INANITY_NET
