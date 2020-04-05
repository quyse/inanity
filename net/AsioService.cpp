#include "AsioService.hpp"
#include "AsioTcpListener.hpp"
#include "AsioTcpSocket.hpp"
#include "AsioInternalUdpSocket.hpp"
#include "AsioUdpListener.hpp"
#include "AsioUdpSocket.hpp"
#include "AsioTlsTcpSocket.hpp"
#include <vector>
#include <sstream>

BEGIN_INANITY_NET

/// Вспомогательный класс запроса на TCP-соединение.
template <typename Socket>
class AsioService::ConnectTcpRequest : public Object
{
private:
	ptr<AsioService> service;
	ptr<Socket> socket;
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
		socket = NEW(Socket(service));

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
		socket->GetLowestSocket().async_connect(*currentEndpointIterator, ConnectedBinder(this));
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
			socket->Close();
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
		service->tcpResolver.async_resolve(
			boost::asio::ip::tcp::resolver::query(
				boost::asio::ip::tcp::v4(),
				host,
				ss.str()
			),
			ResolvedBinder(this)
		);
	}
};

/// Вспомогательный класс запроса на UDP-соединение.
class AsioService::ConnectUdpRequest : public Object
{
private:
	ptr<AsioService> service;
	ptr<AsioInternalUdpSocket> internalSocket;
	ptr<UdpSocketHandler> socketHandler;

	class ResolvedBinder
	{
	private:
		ptr<ConnectUdpRequest> request;

	public:
		ResolvedBinder(ptr<ConnectUdpRequest> request) : request(request) {}

		void operator()(const boost::system::error_code& error, boost::asio::ip::udp::resolver::iterator iterator) const
		{
			request->Resolved(error, iterator);
		}
	};

	boost::asio::ip::udp::resolver::iterator currentEndpointIterator;

	void Resolved(const boost::system::error_code& error, boost::asio::ip::udp::resolver::iterator i)
	{
		if(error)
		{
			socketHandler->FireError(AsioService::ConvertError(error));
			return;
		}

		currentEndpointIterator = i;

		// создать внутренний сокет
		internalSocket = NEW(AsioInternalUdpSocket(service));

		TryConnect();
	}

	void TryConnect()
	{
		// если итератор - последний
		if(currentEndpointIterator == boost::asio::ip::udp::resolver::iterator())
		{
			// значит, ни к одному из endpoints не удалось подключиться
			socketHandler->FireError(NEW(Exception("No luck trying connecting to endpoints")));
			return;
		}

		// пробуем подключиться
		internalSocket->GetSocket().async_connect(*currentEndpointIterator, ConnectedBinder(this));
	}

	class ConnectedBinder
	{
	private:
		ptr<ConnectUdpRequest> request;

	public:
		ConnectedBinder(ptr<ConnectUdpRequest> request) : request(request) {}

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
			internalSocket->GetSocket().close();
			// следующая попытка
			TryConnect();
			return;
		}

		// эгегей, попытка подключения удалась
		ptr<AsioUdpSocket> socket = NEW(AsioUdpSocket(internalSocket, *currentEndpointIterator));
		internalSocket->Start();
		socketHandler->FireData(socket);
	}

public:
	ConnectUdpRequest(ptr<AsioService> service, const String& host, int port, ptr<UdpSocketHandler> socketHandler)
	: service(service), socketHandler(socketHandler)
	{
		// преобразовать порт в строку
		std::ostringstream ss;
		ss << port;
		// разрешить имя хоста
		service->udpResolver.async_resolve(
			boost::asio::ip::udp::resolver::query(
				boost::asio::ip::udp::v4(),
				host,
				ss.str()
			),
			ResolvedBinder(this)
		);
	}
};

class AsioService::TlsHandshakeHandler : public Object
{
private:
	ptr<TcpSocketHandler> socketHandler;

	class HandshakeBinder
	{
	private:
		ptr<TcpSocket> socket;
		ptr<TcpSocketHandler> handler;

	public:
		HandshakeBinder(ptr<TcpSocket> socket, ptr<TcpSocketHandler> handler) : socket(socket), handler(handler) {}

		void operator()(const boost::system::error_code& error) const
		{
			if(error)
			{
				handler->FireError(AsioService::ConvertError(error));
				return;
			}

			handler->FireData(socket);
		}
	};

public:
	TlsHandshakeHandler(ptr<TcpSocketHandler> socketHandler) : socketHandler(socketHandler) {}

	void OnConnect(TcpSocketHandler::Result const& result)
	{
		try
		{
			ptr<AsioTlsTcpSocket> socket = result.GetData().DynamicCast<AsioTlsTcpSocket>();

			socket->GetSocket().async_handshake(Botan::TLS::CLIENT, HandshakeBinder(socket, socketHandler));
		}
		catch(Exception* exception)
		{
			socketHandler->FireError(exception);
		}
	}
};

std::vector<Botan::Certificate_Store*> AsioService::TlsCredentialsManager::trusted_certificate_authorities(std::string const& type, std::string const& hostname)
{
	if(type == "tls-server") return {};
	return { &certStore };
}

AsioService::AsioService()
: work(new boost::asio::io_service::work(ioService)), tcpResolver(ioService), udpResolver(ioService),
tlsSessionManager(Botan::system_rng()),
tlsContext(tlsCredentialsManager, Botan::system_rng(), tlsSessionManager, tlsPolicy)
{}

boost::asio::io_service& AsioService::GetIoService()
{
	return ioService;
}

Botan::TLS::Context& AsioService::GetTlsContext()
{
	return tlsContext;
}

ptr<Exception> AsioService::ConvertError(const boost::system::error_code& error)
{
	std::ostringstream stream;
	stream << "Asio error #" << error.value() << ": " << error.message();
	return NEW(Exception(stream.str()));
}

ptr<Exception> AsioService::ConvertError(const boost::system::system_error& error)
{
	return NEW(Exception(String("Asio error: ") + error.what()));
}

void AsioService::Run()
{
	while(!ioService.stopped())
	{
		ioService.reset();
		ioService.run();
	}
}

void AsioService::Stop()
{
	work.reset();
	ioService.stop();
}

void AsioService::Wait(std::function<void()> handler, int seconds)
{
	std::shared_ptr<boost::asio::deadline_timer> timer = std::make_shared<boost::asio::deadline_timer>(ioService, boost::posix_time::seconds(seconds));
	timer->async_wait([timer, handler](const boost::system::error_code& error)
	{
		handler();
	});
}

ptr<TcpListener> AsioService::ListenTcp(int port, ptr<TcpSocketHandler> socketHandler)
{
	return NEW(AsioTcpListener(this, port, socketHandler));
}

void AsioService::ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler)
{
	MakePointer(NEW(ConnectTcpRequest<AsioTcpSocket>(this, host, port, socketHandler)));
}

ptr<UdpListener> AsioService::ListenUdp(int port, ptr<UdpPacketHandler> receiveHandler)
{
	try
	{
		ptr<AsioInternalUdpSocket> internalSocket = NEW(AsioInternalUdpSocket(this));
		try
		{
			internalSocket->GetSocket().open(boost::asio::ip::udp::v4());
			internalSocket->GetSocket().bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));
		}
		catch(boost::system::system_error error)
		{
			THROW_SECONDARY("Asio error", ConvertError(error));
		}
		ptr<AsioUdpListener> listener = NEW(AsioUdpListener(internalSocket, receiveHandler));
		internalSocket->Start();
		return listener;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't listen UDP", exception);
	}
}

void AsioService::ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> socketHandler)
{
	MakePointer(NEW(ConnectUdpRequest(this, host, port, socketHandler)));
}

void AsioService::ConnectTlsTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler)
{
	MakePointer(NEW(ConnectTcpRequest<AsioTlsTcpSocket>(this, host, port, TcpSocketHandler::Bind(MakePointer(NEW(TlsHandshakeHandler(socketHandler))), &TlsHandshakeHandler::OnConnect))));
}

END_INANITY_NET
