#include "AsioTlsTcpSocket.hpp"
#include "AsioBaseTcpSocket.ipp"
#include "AsioService.hpp"

BEGIN_INANITY_NET

template class AsioBaseTcpSocket<Botan::TLS::Stream<boost::asio::ip::tcp::socket> >;

AsioTlsTcpSocket::AsioTlsTcpSocket(ptr<AsioService> service, std::unique_ptr<Botan::TLS::Context>&& tlsContext)
: AsioBaseTcpSocket(service, service->GetIoService(), *tlsContext), tlsContext(std::move(tlsContext)) {}

void AsioTlsTcpSocket::ShutdownNonSynced()
{
	try
	{
		socket.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_send);
	}
	catch(boost::system::system_error)
	{
	}
}

void AsioTlsTcpSocket::CloseNonSynced()
{
	try
	{
		socket.lowest_layer().close();
	}
	catch(boost::system::system_error error)
	{
	}
}

boost::asio::basic_socket<boost::asio::ip::tcp>& AsioTlsTcpSocket::GetLowestSocket()
{
	return socket.lowest_layer();
}

void AsioTlsTcpSocket::SetNoDelay(bool noDelay)
{
	socket.lowest_layer().set_option(boost::asio::ip::tcp::no_delay(noDelay));
}

END_INANITY_NET
