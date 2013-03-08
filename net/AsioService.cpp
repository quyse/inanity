#include "AsioService.hpp"
#include "AsioTcpListener.hpp"

AsioService::AsioService() : resolver(ioService) {}

boost::asio::io_service& AsioService::GetIoService()
{
	return ioService;
}

ptr<Exception> AsioService::ConvertError(const boost::system::error_code& error)
{
	std::ostringstream stream;
	stream << "Asio error #" << error.value() << ": " << error.message();
	return NEW(Exception(stream.str()));
}

void AsioService::Run()
{
	ioService.run();
}

void AsioService::Stop()
{
	ioService.stop();
}

ptr<TcpListener> AsioService::ListenTcp(int port, ptr<TcpSocketHandler> socketHandler)
{
	try
	{
		return NEW(AsioTcpListener(this, port, socketHandler));
	}
	catch(Exception* exception)
	{
		socketHandler->FireError(NEW(Exception("Can't listen Asio TCP", exception)));
	}
}

void AsioService::ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler)
{
	THROW_PRIMARY_EXCEPTION("Not implemented");
}

void AsioService::ListenUdp(int port, ptr<UdpSocketHandler> socketHandler, ptr<UdpPacketHandler> packetHandler)
{
	THROW_PRIMARY_EXCEPTION("Not implemented");
}

void AsioService::ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> connectHandler, ptr<UdpPacketHandler> packetHandler)
{
	THROW_PRIMARY_EXCEPTION("Not implemented");
}
