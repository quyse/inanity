#ifndef ___INANITY_NET_ASIO_TCP_SOCKET_HPP___
#define ___INANITY_NET_ASIO_TCP_SOCKET_HPP___

#include "TcpSocket.hpp"
#include "asio.hpp"
#include "../CriticalSection.hpp"
#include <deque>

BEGIN_INANITY_NET

class AsioService;

/// Класс TCP-сокета через библиотеку Asio.
class AsioTcpSocket : public TcpSocket
{
public:
	typedef SuccessHandler SendHandler;
	typedef DataHandler<ptr<File> > ReceiveHandler;

private:
	ptr<AsioService> service;
	boost::asio::ip::tcp::socket socket;
	ptr<ReceiveHandler> receiveHandler;

	class Buffers;

	/// Очередь данных на передачу.
	struct SendItem
	{
		ptr<File> data;
		ptr<SendHandler> handler;
		SendItem(ptr<File> data, ptr<SendHandler> handler);
	};
	std::deque<SendItem> sendQueue;
	/// Сколько данных из первого файла в очереди уже передано.
	size_t firstItemSent;
	/// Запланировано ли закрытие передачи.
	bool sendClosed;
	/// Критическая секция для доступа к очереди.
	CriticalSection sendQueueCS;

	/// Размер одного файла для приёма данных.
	static const size_t receiveFileSize;
	/// Файл для приёма данных.
	ptr<File> receiveFile;

	void StartSending();
	void Sent(const boost::system::error_code& error, size_t transferred);
	void StartReceiving();
	void Received(const boost::system::error_code& error, size_t transferred);

public:
	AsioTcpSocket(ptr<AsioService> service);

	boost::asio::ip::tcp::socket& GetSocket();

	//*** Методы TcpSocket.
	void Send(ptr<File> file, ptr<SendHandler> sendHandler);
	void CloseSend();
	void SetReceiveHandler(ptr<ReceiveHandler> receiveHandler);
};

END_INANITY_NET

#endif
