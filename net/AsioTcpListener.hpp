#ifndef ___INANITY_NET_ASIO_TCP_LISTENER_HPP___
#define ___INANITY_NET_ASIO_TCP_LISTENER_HPP___

#include "TcpListener.hpp"
#include "asio.hpp"
#include "../Handler.hpp"

BEGIN_INANITY_NET

class TcpSocket;
class AsioService;
class AsioTcpSocket;

/// Класс серверного TCP-сокета, основанного на библиотеке Asio.
class AsioTcpListener : public TcpListener
{
public:
	typedef DataHandler<ptr<TcpSocket> > SocketHandler;

private:
	ptr<AsioService> service;
	ptr<SocketHandler> socketHandler;
	boost::asio::ip::tcp::acceptor acceptor;
	/// Сокет, который примет очередное входящее соединение.
	/** То есть, просто пока содержит внутренний объект Asio. */
	ptr<AsioTcpSocket> acceptingSocket;

	class AcceptedBinder;

	void StartAccept();
	void Accepted(const boost::system::error_code& error);

public:
	AsioTcpListener(ptr<AsioService> service, int port, ptr<SocketHandler> socketHandler);

	//*** Методы TcpListener.
	void Close() override;
};

END_INANITY_NET

#endif
