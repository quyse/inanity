#ifndef ___INANITY_NET_ASIO_SERVICE_HPP___
#define ___INANITY_NET_ASIO_SERVICE_HPP___

#include "Service.hpp"
#include "asio.hpp"
#include <boost/scoped_ptr.hpp>

BEGIN_INANITY_NET

/// Класс сетевого сервиса на основе библиотеки Boost.Asio.
class AsioService : public Service
{
private:
	boost::asio::io_service ioService;
	boost::scoped_ptr<boost::asio::io_service::work> work;
	boost::asio::ip::tcp::resolver tcpResolver;
	boost::asio::ip::udp::resolver udpResolver;

	class ConnectTcpRequest;
	class ConnectUdpRequest;

public:
	AsioService();

	boost::asio::io_service& GetIoService();

	static ptr<Exception> ConvertError(const boost::system::error_code& error);
	static ptr<Exception> ConvertError(const boost::system::system_error& error);

	//*** Методы Service.
	void Run() override;
	void Stop() override;
	ptr<TcpListener> ListenTcp(int port, ptr<TcpSocketHandler> socketHandler) override;
	void ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler) override;
	ptr<UdpListener> ListenUdp(int port, ptr<UdpPacketHandler> receiveHandler) override;
	void ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> socketHandler) override;
};

END_INANITY_NET

#endif
