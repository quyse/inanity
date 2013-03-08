#ifndef ___INANITY_NET_TCP_SOCKET_HPP___
#define ___INANITY_NET_TCP_SOCKET_HPP___

#include "net.hpp"
#include "../Handler.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_NET

/// Абстрактный класс TCP-сокета.
/** TCP-сокет не предоставляет интерфейса потоков данных, потому что
передача выполняется асинхронно. */
class TcpSocket : public Object
{
public:
	typedef SuccessHandler SendHandler;
	typedef DataHandler<ptr<File> > ReceiveHandler;

public:
	/// Записать данные в сокет.
	/** Данные ставятся в очередь на передачу, если ещё прошлые данные не отправились. */
	virtual void Send(ptr<File> file, ptr<SendHandler> sendHandler) = 0;
	/// Установить обработчик приходящих данных.
	virtual void SetReceiveHandler(ptr<ReceiveHandler> receiveHandler) = 0;
};

END_INANITY_NET

#endif
