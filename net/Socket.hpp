#ifndef ___INANITY_NET_SOCKET_HPP___
#define ___INANITY_NET_SOCKET_HPP___

#include "net.hpp"
#include "uv.hpp"

BEGIN_INANITY_NET

class EventLoop;

/// Класс сетевого TCP-сокета.
class Socket : public Object
{
protected:
	ptr<EventLoop> eventLoop;
	uv_tcp_t* stream;

protected:
	static void CloseCallback(uv_handle_t* handle);

public:
	Socket(ptr<EventLoop> eventLoop, uv_tcp_t* stream);
	~Socket();

	void Close();
};

END_INANITY_NET

#endif
