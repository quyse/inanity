#ifndef ___INANITY_NET_UDP_LISTENER_HPP___
#define ___INANITY_NET_UDP_LISTENER_HPP___

#include "net.hpp"
#include "../Handler.hpp"

BEGIN_INANITY_NET

class UdpPacket;

/// Абстрактный класс UDP-слушателя.
/** Конечно, в UDP отсутствует понятие соединения. Но, этот класс введён
для поддержки "соединений", которые представляют собой двунаправленный
обмен UDP-пакетами. Первый пакет, приходящий в UDP-порт, принимается
слушателем, и передаётся обработчику. Обработчик же принимает решение о
том, открывать ли выделенный UDP-сокет для обработки пакетов от этого
клиента. */
class UdpListener : public Object
{
public:
	typedef DataHandler<ptr<UdpPacket> > ReceiveHandler;

public:
	/// Закрыть сокет.
	virtual void Close() = 0;
};

END_INANITY_NET

#endif
