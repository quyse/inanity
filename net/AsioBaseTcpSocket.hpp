#ifndef ___INANITY_NET_ASIO_BASE_TCP_SOCKET_HPP___
#define ___INANITY_NET_ASIO_BASE_TCP_SOCKET_HPP___

#include "TcpSocket.hpp"
#include "asio.hpp"
#include "../CriticalSection.hpp"
#include <deque>

BEGIN_INANITY_NET

class AsioService;

/// Base TCP socket class for Asio.
template <typename Socket>
class AsioBaseTcpSocket : public TcpSocket
{
public:
	typedef SuccessHandler SendHandler;

protected:
	ptr<AsioService> service;
	Socket socket;
	ptr<ReceiveHandler> receiveHandler;

	CriticalSection cs;

	class Buffers;

	/// Sending queue.
	struct SendItem
	{
		ptr<File> data;
		ptr<SendHandler> handler;
		SendItem(ptr<File> data, ptr<SendHandler> handler);
	};
	std::deque<SendItem> sendQueue;
	/// How much data from the first item is sent already.
	size_t firstItemSent;
	/// Closing is planned?
	bool sendClosed;

	static const size_t receiveFileSize = 0x1000;
	ptr<File> receiveFile;

	class SentBinder;
	class ReceivedBinder;

	void StartSending();
	void Sent(const boost::system::error_code& error, size_t transferred);
	void StartReceiving();
	void Received(const boost::system::error_code& error, size_t transferred);

	virtual void ShutdownNonSynced() = 0;
	virtual void CloseNonSynced() = 0;

public:
	template <typename... Args>
	AsioBaseTcpSocket(ptr<AsioService> service, Args&& ...args);

	Socket& GetSocket();
	virtual boost::asio::basic_socket<boost::asio::ip::tcp>& GetLowestSocket() = 0;

	//*** TcpSocket's methods.
	void Send(ptr<File> file, ptr<SendHandler> sendHandler) override;
	void End() override;
	void SetReceiveHandler(ptr<ReceiveHandler> receiveHandler) override;
	void Close() override;
};

END_INANITY_NET

#endif
