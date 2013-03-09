#ifndef ___INANITY_NET_ASIO_SERVICE_HPP___
#define ___INANITY_NET_ASIO_SERVICE_HPP___

#include "Service.hpp"
#include "asio.hpp"

BEGIN_INANITY_NET

/// Класс сетевого сервиса на основе библиотеки Boost.Asio.
class AsioService : public Service
{
private:
	boost::asio::io_service ioService;
	boost::asio::ip::tcp::resolver resolver;

	class ConnectTcpRequest;

public:
	AsioService();

	boost::asio::io_service& GetIoService();

	static ptr<Exception> ConvertError(const boost::system::error_code& error);

	//*** Методы Service.
	void Run();
	void Stop();
	ptr<TcpListener> ListenTcp(int port, ptr<TcpSocketHandler> socketHandler);
	void ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler);
	void ListenUdp(int port, ptr<UdpSocketHandler> socketHandler, ptr<UdpPacketHandler> packetHandler);
	void ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> connectHandler, ptr<UdpPacketHandler> packetHandler);
};

END_INANITY_NET

#endif
