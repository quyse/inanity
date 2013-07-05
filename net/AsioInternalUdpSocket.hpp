#ifndef ___INANITY_NET_ASIO_INTERNAL_UDP_SOCKET_HPP___
#define ___INANITY_NET_ASIO_INTERNAL_UDP_SOCKET_HPP___

#include "net.hpp"
#include "asio.hpp"
#include "../Handler.hpp"
#include "../CriticalSection.hpp"
#include <map>

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_NET

class AsioService;
class AsioUdpListener;
class AsioUdpSocket;

/// Вспомогательный класс, служащий диспетчером для приходящих пакетов.
/** Обслуживает один UDP-сокет Asio, и передаёт пакеты конкретным сокетам.
Нужен для того, чтобы сервер (AsioUdpListener) и клиентские сокеты на сервере
(AsioUdpSocket) могли работать через один реальный UDP-сокет.
*/
class AsioInternalUdpSocket : public Object
{
private:
	ptr<AsioService> service;
	boost::asio::ip::udp::socket socket;

	/// Слушатель.
	/** Обрабатывает все пакеты, которые не достались сокетам. */
	ptr<AsioUdpListener> clientListener;
	/// Сокеты.
	typedef std::map<boost::asio::ip::udp::endpoint, ptr<AsioUdpSocket> > ClientSockets;
	ClientSockets clientSockets;

	/// Размер одного файла для приёма пакетов.
	static const size_t receiveFileSize;
	/// Файл для приёма пакетов.
	ptr<File> receiveFile;
	/// Адрес отправителя принятого пакета.
	boost::asio::ip::udp::endpoint receiveEndpoint;

	CriticalSection cs;

	class ReceiveBinder;

	void StartReceive();
	void Received(const boost::system::error_code& error, size_t transferred);

public:
	AsioInternalUdpSocket(ptr<AsioService> service);

	boost::asio::ip::udp::socket& GetSocket();
	CriticalSection& GetCriticalSection();

	void Start();

	/// Установить слушатель.
	void SetClientListener(ptr<AsioUdpListener> clientListener);
	/// Зарегистрировать сокет для приёма данных с заданного endpoint'а.
	void RegisterClientSocket(const boost::asio::ip::udp::endpoint& endpoint, ptr<AsioUdpSocket> clientSocket);
	/// Разрегистрировать сокет.
	void UnregisterClientSocket(const boost::asio::ip::udp::endpoint& endpoint);

	/// Закрыть сокет.
	void Close();
};

END_INANITY_NET

#endif
