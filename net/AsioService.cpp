#include "AsioService.hpp"
#include "AsioTcpListener.hpp"

AsioService::AsioService() : resolver(ioService) {}

boost::asio::io_service& AsioService::GetIoService()
{
	return ioService;
}

ptr<Exception> AsioService::ConvertError(const boost::system::error_code& error)
{
	return NEW(Exception("Asio error: " + error.message()));
}

void AsioService::Run()
{
	ioService.run();
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
