#include "AsioTcpSocket.hpp"
#include "AsioService.hpp"
#include "../MemoryFile.hpp"
#include "../PartFile.hpp"
#include "../CriticalCode.hpp"
#include <boost/bind.hpp>

BEGIN_INANITY_NET

const size_t AsioTcpSocket::receiveFileSize = 0x1000;

AsioTcpSocket::SendItem::SendItem(ptr<File> data, ptr<SendHandler> handler)
: data(data), handler(handler) {}

/// Вспомогательный класс последовательности буферов.
/** Удовлетворяет требованиям Asio к ConstBufferSequence. */
class AsioTcpSocket::Buffers
{
private:
	typedef std::deque<SendItem>::const_iterator InternalIterator;

public:
	/// Класс буфера.
	class Buffer
	{
	private:
		/// Файл с данными.
		ptr<File> file;
		/// Смещение от начала, которое нужно пропустить.
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

	/// Класс константного итератора.
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
	// Границы посылки.
	InternalIterator beginIt, endIt;
	/// Сколько следует пропустить от начала первого буфера.
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

/// Вспомогательный класс для обработчика завершения отправки.
class AsioTcpSocket::SentBinder
{
private:
	ptr<AsioTcpSocket> socket;

public:
	SentBinder(ptr<AsioTcpSocket> socket) : socket(socket) {}

	void operator()(const boost::system::error_code& error, size_t transferred) const
	{
		socket->Sent(error, transferred);
	}
};

/// Вспомогательный класс для обработчика принятия данных.
class AsioTcpSocket::ReceivedBinder
{
private:
	ptr<AsioTcpSocket> socket;

public:
	ReceivedBinder(ptr<AsioTcpSocket> socket) : socket(socket) {}

	void operator()(const boost::system::error_code& error, size_t transferred) const
	{
		socket->Received(error, transferred);
	}
};

AsioTcpSocket::AsioTcpSocket(ptr<AsioService> service)
	: service(service), socket(service->GetIoService()),
	firstItemSent(0), sendClosed(false) {}

boost::asio::ip::tcp::socket& AsioTcpSocket::GetSocket()
{
	return socket;
}

void AsioTcpSocket::StartSending()
{
	// если в очереди ничего нет, проверить, не нужно ли закрыть сокет
	if(sendQueue.empty())
	{
		if(sendClosed)
			try
			{
				socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
			}
			catch(boost::system::system_error error)
			{
				THROW_SECONDARY_EXCEPTION("Can't close Asio TCP socket", AsioService::ConvertError(error));
			}
	}
	else
	{
		// очередь не пуста, начинаем отправку
		socket.async_write_some(Buffers(sendQueue.begin(), sendQueue.end(), firstItemSent), SentBinder(this));
	}
}

void AsioTcpSocket::Sent(const boost::system::error_code& error, size_t transferred)
{
	// если произошла ошибка
	if(error)
	{
		ptr<Exception> exception = AsioService::ConvertError(error);

		// отправить её всем элементам в очереди, и очистить очередь

		// скопируем обработчики в вектор, чтобы вызывать их
		// вне блокировки, и обеспечить реентрантность
		std::vector<ptr<SendHandler> > handlers;
		{
			CriticalCode cc(cs);
			handlers.resize(sendQueue.size());
			std::deque<SendItem>::iterator i;
			size_t j = 0;
			for(i = sendQueue.begin(); i != sendQueue.end(); ++i)
				handlers[j++] = i->handler;
			sendQueue.clear();
		}

		for(size_t j = 0; j < handlers.size(); ++j)
			if(handlers[j])
				handlers[j]->FireError(exception);

		return;
	}
	// ошибки нет
	else
	{
		// уведомить все завершившиеся файлы, и удалить их из очереди

		// скопируем обработчики в вектор, чтобы вызывать их
		// вне блокировки, и обеспечить реентрантность
		std::vector<ptr<SendHandler> > handlers;
		{
			CriticalCode cc(cs);
			std::deque<SendItem>::iterator i;
			size_t j = 0;
			for(i = sendQueue.begin(); i != sendQueue.end(); ++i)
			{
				const SendItem& item = *i;
				// если элемент не передался полностью, закончить
				size_t itemDataSkip = i == sendQueue.begin() ? firstItemSent : 0;
				size_t itemSize = item.data->GetSize() - itemDataSkip;
				if(itemSize > transferred)
				{
					// этот элемент теперь будет первым, выставить новое смещение
					firstItemSent = itemDataSkip + transferred;
					break;
				}
				// иначе передался полностью
				transferred -= itemSize;
				// добавить обработчик в массив
				if(item.handler)
					handlers.push_back(item.handler);
			}
			// теперь i указывает на первый элемент, который ещё не передался
			// удалить всё, что до него
			sendQueue.erase(sendQueue.begin(), i);

			// если в очереди что-то есть, запустить отправку снова
			StartSending();
		}

		for(size_t j = 0; j < handlers.size(); ++j)
			handlers[j]->FireSuccess();
	}
}

void AsioTcpSocket::StartReceiving()
{
	receiveFile = NEW(MemoryFile(receiveFileSize));
	socket.async_read_some(boost::asio::buffer(receiveFile->GetData(), receiveFileSize), ReceivedBinder(this));
}

void AsioTcpSocket::Received(const boost::system::error_code& error, size_t transferred)
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
			this->receiveHandler = 0;
			this->receiveFile = 0;
		}
		else
			StartReceiving();
	}

	if(receiveHandler)
	{
		if(error)
		{
			// если корректный конец файла, это не ошибка
			if(error == boost::asio::error::eof)
				receiveHandler->FireData(0);
			else
				receiveHandler->FireError(AsioService::ConvertError(error));
		}
		else
			receiveHandler->FireData(NEW(PartFile(receiveFile, receiveFile->GetData(), transferred)));
	}
}

void AsioTcpSocket::CloseNonSynced()
{
	try
	{
		socket.close();
	}
	catch(boost::system::system_error error)
	{
	}
}

void AsioTcpSocket::Send(ptr<File> file, ptr<SendHandler> sendHandler)
{
	try
	{
		CriticalCode cc(cs);

		// если запланировано закрытие передачи, то больше в очередь добавлять ничего нельзя
		if(sendClosed)
			THROW_PRIMARY_EXCEPTION("Sending closed");

		bool queueWasEmpty = sendQueue.empty();

		// добавить элемент в очередь
		sendQueue.push_back(SendItem(file, sendHandler));

		// сбросить количество переданных данных в первом элементе,
		// если очередь была пуста (то есть первый элемент как раз
		// был добавлен)
		// также, если очередь была пуста, начать отправку
		if(queueWasEmpty)
		{
			firstItemSent = 0;
			StartSending();
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't send data to Asio TCP socket", exception);
	}
}

void AsioTcpSocket::End()
{
	CriticalCode cc(cs);

	if(!sendClosed)
	{
		sendClosed = true;

		if(sendQueue.empty())
			StartSending();
	}
}

void AsioTcpSocket::SetReceiveHandler(ptr<ReceiveHandler> receiveHandler)
{
	CriticalCode cc(cs);

	bool firstTime = !this->receiveHandler;
	this->receiveHandler = receiveHandler;

	if(firstTime)
		// запустить приём
		StartReceiving();
}

void AsioTcpSocket::Close()
{
	CriticalCode cc(cs);

	CloseNonSynced();
}

END_INANITY_NET
