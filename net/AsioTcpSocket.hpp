#ifndef ___INANITY_NET_ASIO_TCP_SOCKET_HPP___
#define ___INANITY_NET_ASIO_TCP_SOCKET_HPP___

#include "AsioBaseTcpSocket.hpp"

BEGIN_INANITY_NET

class AsioService;

extern template class AsioBaseTcpSocket<boost::asio::ip::tcp::socket>;

/// TCP socket via Asio.
class AsioTcpSocket : public AsioBaseTcpSocket<boost::asio::ip::tcp::socket>
{
protected:
	//*** AsioBaseTcpSocket's methods.
	void ShutdownNonSynced() override;
	void CloseNonSynced() override;

public:
	AsioTcpSocket(ptr<AsioService> service);

	//*** AsioBaseTcpSocket's methods.
	boost::asio::ip::tcp::socket& GetLowestSocket() override;

	//*** TcpSocket's methods.
	void SetNoDelay(bool noDelay) override;
};

END_INANITY_NET

#endif
