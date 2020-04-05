#ifndef ___INANITY_NET_ASIO_BASE_TCP_SOCKET_IPP___
#define ___INANITY_NET_ASIO_BASE_TCP_SOCKET_IPP___

#include "AsioBaseTcpSocket.hpp"
#include "AsioService.hpp"
#include "../MemoryFile.hpp"
#include "../PartFile.hpp"
#include "../CriticalCode.hpp"
#include <boost/bind.hpp>

BEGIN_INANITY_NET

template <typename Socket>
AsioBaseTcpSocket<Socket>::SendItem::SendItem(ptr<File> data, ptr<SendHandler> handler)
: data(data), handler(handler) {}

/// Helper class for buffer sequence.
/** Conforms to Asio's ConstBufferSequence. */
template <typename Socket>
class AsioBaseTcpSocket<Socket>::Buffers
{
private:
	typedef typename std::deque<SendItem>::const_iterator InternalIterator;

public:
	class Buffer
	{
	private:
		ptr<File> file;
		/// Offset to skip.
		size_t skip;

	public:
		Buffer(ptr<File> file, size_t skip) : file(file), skip(skip) {}

		operator boost::asio::const_buffer() const
		{
			return boost::asio::const_buffer((const void*)*this, (size_t)*this);
		}

		operator const void*() const
		{
			return (const char*)file->GetData() + skip;
		}

		operator size_t() const
		{
			return file->GetSize() - skip;
		}
	};

	typedef Buffer value_type;

	class ConstIterator
	{
	private:
		const Buffers& buffers;
		InternalIterator it;

	public:
		ConstIterator(const Buffers& buffers, InternalIterator it) : buffers(buffers), it(it) {}

		const Buffer operator*() const
		{
			return Buffer(it->data, it == buffers.beginIt ? buffers.firstSkip : 0);
		}
		ConstIterator& operator++()
		{
			++it;
			return *this;
		}
		ConstIterator operator++(int)
		{
			ConstIterator temp(*this);
			++it;
			return temp;
		}
		ConstIterator& operator--()
		{
			--it;
			return *this;
		}
		ConstIterator operator--(int)
		{
			ConstIterator temp(*this);
			--it;
			return temp;
		}
		friend bool operator==(const ConstIterator& a, const ConstIterator& b)
		{
			return a.it == b.it;
		}
		friend bool operator!=(const ConstIterator& a, const ConstIterator& b)
		{
			return a.it != b.it;
		}
	};

	typedef ConstIterator const_iterator;

private:
	// Sending limits.
	InternalIterator beginIt, endIt;
	/// How much to skip from the beginning of the first buffer.
	size_t firstSkip;

public:
	Buffers(InternalIterator begin, InternalIterator end, size_t firstSkip)
	: beginIt(begin), endIt(end), firstSkip(firstSkip) {}

	ConstIterator begin() const
	{
		return ConstIterator(*this, beginIt);
	}
	ConstIterator end() const
	{
		return ConstIterator(*this, endIt);
	}
};

/// Helper class for sending handler.
template <typename Socket>
class AsioBaseTcpSocket<Socket>::SentBinder
{
private:
	ptr<AsioBaseTcpSocket> socket;

public:
	SentBinder(ptr<AsioBaseTcpSocket> socket) : socket(socket) {}

	void operator()(const boost::system::error_code& error, size_t transferred) const
	{
		socket->Sent(error, transferred);
	}
};

/// Helper class for receiving handler.
template <typename Socket>
class AsioBaseTcpSocket<Socket>::ReceivedBinder
{
private:
	ptr<AsioBaseTcpSocket> socket;

public:
	ReceivedBinder(ptr<AsioBaseTcpSocket> socket) : socket(socket) {}

	void operator()(const boost::system::error_code& error, size_t transferred) const
	{
		socket->Received(error, transferred);
	}
};

template <typename Socket>
template <typename... Args>
AsioBaseTcpSocket<Socket>::AsioBaseTcpSocket(ptr<AsioService> service, Args&& ...args)
	: service(service), socket(args...), firstItemSent(0), sendClosed(false) {}

template <typename Socket>
Socket& AsioBaseTcpSocket<Socket>::GetSocket()
{
	return socket;
}

template <typename Socket>
void AsioBaseTcpSocket<Socket>::StartSending()
{
	// if queue is empty, check if socket must be closed
	if(sendQueue.empty())
	{
		if(sendClosed)
			ShutdownNonSynced();
	}
	else
	{
		// queue is not empty, start sending
		socket.async_write_some(Buffers(sendQueue.begin(), sendQueue.end(), firstItemSent), SentBinder(this));
	}
}

template <typename Socket>
void AsioBaseTcpSocket<Socket>::Sent(const boost::system::error_code& error, size_t transferred)
{
	if(error)
	{
		ptr<Exception> exception = AsioService::ConvertError(error);

		// send the error to every item, and clear the queue

		// copy handlers to vector, to call them outside of lock,
		// and for proper re-entrancy
		std::vector<ptr<SendHandler> > handlers;
		{
			CriticalCode cc(cs);
			handlers.resize(sendQueue.size());
			size_t j = 0;
			for(typename std::deque<SendItem>::iterator i = sendQueue.begin(); i != sendQueue.end(); ++i)
				handlers[j++] = i->handler;
			sendQueue.clear();
		}

		for(size_t j = 0; j < handlers.size(); ++j)
			if(handlers[j])
				handlers[j]->FireError(exception);

		return;
	}
	// no error
	else
	{
		// notify all completed files, and remove them

		// copy handlers to vector, to call them outside of lock,
		// and for proper re-entrancy
		std::vector<ptr<SendHandler> > handlers;
		{
			CriticalCode cc(cs);
			typename std::deque<SendItem>::iterator i;
			for(i = sendQueue.begin(); i != sendQueue.end(); ++i)
			{
				const SendItem& item = *i;
				// if item is not completed
				size_t itemDataSkip = i == sendQueue.begin() ? firstItemSent : 0;
				size_t itemSize = item.data->GetSize() - itemDataSkip;
				if(itemSize > transferred)
				{
					// the item is now the first
					firstItemSent = itemDataSkip + transferred;
					break;
				}
				// else it's completed
				transferred -= itemSize;
				if(item.handler)
					handlers.push_back(item.handler);
			}
			// now i points to the first not completed item
			// remove everything before it
			sendQueue.erase(sendQueue.begin(), i);

			// start sending again if queue is not empty
			StartSending();
		}

		for(size_t j = 0; j < handlers.size(); ++j)
			handlers[j]->FireSuccess();
	}
}

template <typename Socket>
void AsioBaseTcpSocket<Socket>::StartReceiving()
{
	receiveFile = NEW(MemoryFile(receiveFileSize));
	socket.async_read_some(boost::asio::buffer(receiveFile->GetData(), receiveFileSize), ReceivedBinder(this));
}

template <typename Socket>
void AsioBaseTcpSocket<Socket>::Received(const boost::system::error_code& error, size_t transferred)
{
	ptr<ReceiveHandler> receiveHandler;
	ptr<File> receiveFile;

	{
		CriticalCode cc(cs);

		receiveHandler = this->receiveHandler;
		receiveFile = this->receiveFile;

		if(error)
		{
			CloseNonSynced();
			this->receiveHandler = nullptr;
			this->receiveFile = nullptr;
		}
		else
			StartReceiving();
	}

	if(receiveHandler)
	{
		if(error)
		{
			// EOF is not an error
			if(error == boost::asio::error::eof)
				receiveHandler->FireData(nullptr);
			else
				receiveHandler->FireError(AsioService::ConvertError(error));
		}
		else
			receiveHandler->FireData(NEW(PartFile(receiveFile, receiveFile->GetData(), transferred)));
	}
}

template <typename Socket>
void AsioBaseTcpSocket<Socket>::Send(ptr<File> file, ptr<SendHandler> sendHandler)
{
	BEGIN_TRY();

	CriticalCode cc(cs);

	// no more sending if it's planned to close
	if(sendClosed)
		THROW("Sending closed");

	bool queueWasEmpty = sendQueue.empty();

	sendQueue.push_back(SendItem(file, sendHandler));

	// start sending if needed
	if(queueWasEmpty)
	{
		firstItemSent = 0;
		StartSending();
	}

	END_TRY("Can't send data to Asio TCP socket");
}

template <typename Socket>
void AsioBaseTcpSocket<Socket>::End()
{
	CriticalCode cc(cs);

	if(!sendClosed)
	{
		sendClosed = true;

		if(sendQueue.empty())
			StartSending();
	}
}

template <typename Socket>
void AsioBaseTcpSocket<Socket>::SetReceiveHandler(ptr<ReceiveHandler> receiveHandler)
{
	CriticalCode cc(cs);

	bool firstTime = !this->receiveHandler;
	this->receiveHandler = receiveHandler;

	if(firstTime)
		StartReceiving();
}

template <typename Socket>
void AsioBaseTcpSocket<Socket>::Close()
{
	CriticalCode cc(cs);

	CloseNonSynced();
}

END_INANITY_NET

#endif
