#ifndef ___INANITY_NET_EVENT_LOOP_HPP___
#define ___INANITY_NET_EVENT_LOOP_HPP___

#include "net.hpp"
#include "asio.hpp"
#include "ServerSocket.hpp"
#include "../Handler.hpp"
#include "../String.hpp"

BEGIN_INANITY

class Exception;

END_INANITY

BEGIN_INANITY_NET

class ClientSocket;

/// Класс цикла обработки асинхронных сообщений для библиотеки libuv.
class EventLoop : public Object
{
	friend class ServerSocket;
	friend class ClientSocket;
public:
	typedef Handler<ptr<ClientSocket> > ConnectHandler;

private:
	typedef Handler<const std::vector<asio::ip::tcp::endpoint>&> ResolveHandler;

	asio::io_service ioService;
	asio::tcp::resolver resolver;

	/// Класс запроса на разрешение имени.
	class ResolveRequest : public Object
	{
	private:
		ptr<EventLoop> eventLoop;
		ptr<ResolveHandler> resolveHandler;

	public:
		ResolveRequest(ptr<EventLoop> eventLoop, ptr<ResolveHandler> resolveHandler);
		void Handle(const asio::error_code& error, resolver::iterator iterator);
	};

	/// Класс запроса на соединение.
	class ConnectRequest : public Object
	{
	private:
		ptr<EventLoop> eventLoop;
		ptr<ConnectHandler> connectHandler;
		std::vector<asio::ip::tcp::endpoint> endpoints;
		size_t currentEndpointIndex;
		ptr<ClientSocket> clientSocket;

		void TryConnect();
		void HandleTryConnect(const asio::error_code& error);

	public:
		ConnectRequest(ptr<EventLoop> eventLoop, ptr<ConnectHandler> connectHandler);

		void HandleResolveResult(const std::vector<asio::ip::tcp::endpoint>& endpoints);
	};

	/// Разрешить имя.
	void Resolve(const String& host, int port, ptr<ResolveHandler> resolveHandler);

public:
	EventLoop();
	~EventLoop();

	asio::io_service& GetIoService();
	ptr<Exception> GetLastError();

	/// Открыть серверный сокет для приёма входящих соединений.
	ptr<ServerSocket> Listen(int port, ptr<ServerSocket::SocketHandler> listenHandler);
	/// Подключиться к серверу.
	void Connect(const String& host, int port, ptr<ConnectHandler> connectHandler);

	/// Запустить цикл обработки сообщений.
	void Run();

	/// Выбросить исключение, соответствующее ошибке ASIO.
	void HandleAsioError(const asio::system_error& error);
};

END_INANITY_NET

#endif
