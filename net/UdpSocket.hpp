#ifndef ___INANITY_NET_UDP_SOCKET_HPP___
#define ___INANITY_NET_UDP_SOCKET_HPP___

#include "net.hpp"
#include "../Handler.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_NET

/// Абстрактный класс UDP-сокета.
class UdpSocket : public Object
{
public:
	/// Тип обработчика входящих данных.
	typedef DataHandler<ptr<File> > ReceiveHandler;

public:
	/// Отправить пакет данных в сокет.
	virtual void Send(ptr<File> file) = 0;
	/// Установить обработчик приходящих данных.
	virtual void SetReceiveHandler(ptr<ReceiveHandler> receiveHandler) = 0;
	/// Закрыть принимающую сторону сокета.
	virtual void Close() = 0;
};

END_INANITY_NET

#endif
