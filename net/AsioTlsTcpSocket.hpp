#ifndef ___INANITY_NET_ASIO_TLS_TCP_SOCKET_HPP___
#define ___INANITY_NET_ASIO_TLS_TCP_SOCKET_HPP___

#include "AsioBaseTcpSocket.hpp"
#include <botan/asio_stream.h>

BEGIN_INANITY_NET

class AsioService;

extern template class AsioBaseTcpSocket<Botan::TLS::Stream<boost::asio::ip::tcp::socket> >;

/// TCP-socket with TLS via Boost.Asio and Botan.
class AsioTlsTcpSocket : public AsioBaseTcpSocket<Botan::TLS::Stream<boost::asio::ip::tcp::socket> >
{
protected:
	std::unique_ptr<Botan::TLS::Context> tlsContext;

	//*** AsioBaseTcpSocket's methods.
	void ShutdownNonSynced() override;
	void CloseNonSynced() override;

public:
	AsioTlsTcpSocket(ptr<AsioService> service, std::unique_ptr<Botan::TLS::Context>&& tlsContext);

	//*** AsioBaseTcpSocket's methods.
	boost::asio::basic_socket<boost::asio::ip::tcp>& GetLowestSocket() override;

	//*** TcpSocket's methods.
	void SetNoDelay(bool noDelay) override;
};

END_INANITY_NET

#endif
