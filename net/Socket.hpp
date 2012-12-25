#ifndef ___INANITY_NET_SOCKET_HPP___
#define ___INANITY_NET_SOCKET_HPP___

#include "net.hpp"

BEGIN_INANITY_NET

class EventLoop;

/// Класс сетевого TCP-сокета.
class Socket : public Object
{
protected:
	ptr<EventLoop> eventLoop;
	asio::tcp::socket socket;

public:
	Socket(ptr<EventLoop> eventLoop);
	~Socket();

	void Close();
};

END_INANITY_NET

#endif
