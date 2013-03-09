#include "AsioService.hpp"
#include "AsioTcpListener.hpp"
#include "AsioTcpSocket.hpp"
#include <vector>
#include <sstream>

/// Вспомогательный класс запроса на соединение.
class AsioService::ConnectTcpRequest : public Object
{
private:
	ptr<AsioService> service;
	ptr<AsioTcpSocket> socket;
	ptr<TcpSocketHandler> socketHandler;

	class ResolvedBinder
	{
	private:
		ptr<ConnectTcpRequest> request;

	public:
		ResolvedBinder(ptr<ConnectTcpRequest> request) : request(request) {}

		void operator()(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iterator) const
		{
			request->Resolved(error, iterator);
		}
	};

	boost::asio::ip::tcp::resolver::iterator currentEndpointIterator;

	void Resolved(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator i)
	{
		if(error)
		{
			socketHandler->FireError(AsioService::ConvertError(error));
			return;
		}

		currentEndpointIterator = i;

		// создать сокет
		socket = NEW(AsioTcpSocket(service));

		TryConnect();
	}

	void TryConnect()
	{
		// если итератор - последний
		if(currentEndpointIterator == boost::asio::ip::tcp::resolver::iterator())
		{
			// значит, ни к одному из endpoints не удалось подключиться
			socketHandler->FireError(NEW(Exception("No luck trying connecting to endpoints")));
			return;
		}

		// пробуем подключиться
		socket->GetSocket().async_connect(*currentEndpointIterator, ConnectedBinder(this));
	}

	class ConnectedBinder
	{
	private:
		ptr<ConnectTcpRequest> request;

	public:
		ConnectedBinder(ptr<ConnectTcpRequest> request) : request(request) {}

		void operator()(const boost::system::error_code& error) const
		{
			request->Connected(error);
		}
	};

	void Connected(const boost::system::error_code& error)
	{
		// если попытка подключения не удалась
		if(error)
		{
			// пробуем следующий endpoint
			++currentEndpointIterator;
			// закрываем сокет, чтобы повторно подключиться
			socket->GetSocket().close();
			// следующая попытка
			TryConnect();
			return;
		}

		// эгегей, попытка подключения удалась
		socketHandler->FireData(socket);
	}

public:
	ConnectTcpRequest(ptr<AsioService> service, const String& host, int port, ptr<TcpSocketHandler> socketHandler)
	: service(service), socketHandler(socketHandler)
	{
		// преобразовать порт в строку
		std::ostringstream ss;
		ss << port;
		// разрешить имя хоста
		service->resolver.async_resolve(
			boost::asio::ip::tcp::resolver::query(
				boost::asio::ip::tcp::v4(),
				host,
				ss.str()
			),
			ResolvedBinder(this)
		);
	}
};

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
	return NEW(AsioTcpListener(this, port, socketHandler));
}

void AsioService::ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler)
{
	MakePointer(NEW(ConnectTcpRequest(this, host, port, socketHandler)));
}

void AsioService::ListenUdp(int port, ptr<UdpSocketHandler> socketHandler, ptr<UdpPacketHandler> packetHandler)
{
	THROW_PRIMARY_EXCEPTION("Not implemented");
}

void AsioService::ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> connectHandler, ptr<UdpPacketHandler> packetHandler)
{
	THROW_PRIMARY_EXCEPTION("Not implemented");
}
