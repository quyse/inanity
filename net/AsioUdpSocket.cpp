#include "AsioUdpSocket.hpp"
#include "AsioService.hpp"
#include "AsioInternalUdpSocket.hpp"
#include "../File.hpp"
#include "../CriticalCode.hpp"

BEGIN_INANITY_NET

/// Вспомогательный класс буфера.
/** Представляет собой один буфер с данными.
Удовлетворяет требованиям Asio к ConstBufferSequence. */
class AsioUdpSocket::Buffers1
{
public:
	/// Класс буфера.
	class Buffer
	{
	private:
		/// Файл с данными.
		ptr<File> file;

	public:
		Buffer(ptr<File> file) : file(file) {}

		operator boost::asio::const_buffer() const
		{
			return boost::asio::const_buffer((const void*)*this, (size_t)*this);
		}

		operator const void*() const
		{
			return (const char*)file->GetData();
		}

		operator size_t() const
		{
			return file->GetSize();
		}
	};

	typedef Buffer value_type;

	/// Класс константного итератора.
	class ConstIterator
	{
	private:
		const Buffers1& buffers;
		bool end;

	public:
		ConstIterator(const Buffers1& buffers, bool end) : buffers(buffers), end(end) {}

		const Buffer operator*() const
		{
			return Buffer(buffers.file);
		}
		ConstIterator& operator++()
		{
			end = true;
			return *this;
		}
		ConstIterator operator++(int)
		{
			ConstIterator temp(*this);
			end = true;
			return temp;
		}
		ConstIterator& operator--()
		{
			end = false;
			return *this;
		}
		ConstIterator operator--(int)
		{
			ConstIterator temp(*this);
			end = false;
			return temp;
		}
		friend bool operator==(const ConstIterator& a, const ConstIterator& b)
		{
			return a.end == b.end;
		}
		friend bool operator!=(const ConstIterator& a, const ConstIterator& b)
		{
			return a.end != b.end;
		}
	};

	typedef ConstIterator const_iterator;

private:
	/// Файл с данными.
	ptr<File> file;

public:
	Buffers1(ptr<File> file) : file(file) {}

	ConstIterator begin() const
	{
		return ConstIterator(*this, false);
	}
	ConstIterator end() const
	{
		return ConstIterator(*this, true);
	}
};

class AsioUdpSocket::SentBinder
{
private:
	ptr<AsioUdpSocket> socket;

public:
	SentBinder(ptr<AsioUdpSocket> socket) : socket(socket) {}

	void operator()(const boost::system::error_code& error, size_t transferred) const
	{
		socket->Sent(error, transferred);
	}
};

AsioUdpSocket::AsioUdpSocket(ptr<AsioInternalUdpSocket> internalSocket, const boost::asio::ip::udp::endpoint& remoteEndpoint)
: internalSocket(internalSocket), remoteEndpoint(remoteEndpoint)
{
	internalSocket->RegisterClientSocket(remoteEndpoint, this);
}

void AsioUdpSocket::Sent(const boost::system::error_code& error, size_t transferred)
{
	if(error)
		Close();
}

void AsioUdpSocket::Receive(ptr<File> file)
{
	ptr<ReceiveHandler> receiveHandler;

	{
		CriticalCode cc(cs);

		receiveHandler = this->receiveHandler;
	}

	if(receiveHandler)
		receiveHandler->FireData(file);
}

void AsioUdpSocket::ReceiveError(const boost::system::error_code& error)
{
	ptr<ReceiveHandler> receiveHandler;

	{
		CriticalCode cc(cs);

		receiveHandler = this->receiveHandler;
	}

	Close();

	if(receiveHandler)
		receiveHandler->FireError(AsioService::ConvertError(error));
}

void AsioUdpSocket::Send(ptr<File> file)
{
	try
	{
		CriticalCode cc(cs);
		CriticalCode cc2(internalSocket->GetCriticalSection());

		internalSocket->GetSocket().async_send_to(Buffers1(file), remoteEndpoint, SentBinder(this));
	}
	catch(boost::system::system_error error)
	{
		THROW_SECONDARY_EXCEPTION("Can't send file", AsioService::ConvertError(error));
	}
}

void AsioUdpSocket::SetReceiveHandler(ptr<ReceiveHandler> receiveHandler)
{
	CriticalCode cc(cs);

	if(this->receiveHandler)
		THROW_PRIMARY_EXCEPTION("Receive handler already set");

	this->receiveHandler = receiveHandler;
}

void AsioUdpSocket::Close()
{
	CriticalCode cc(cs);

	if(internalSocket)
	{
		internalSocket->UnregisterClientSocket(remoteEndpoint);
		internalSocket = 0;
	}
}

END_INANITY_NET
