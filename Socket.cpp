#include "Socket.hpp"
#include "EventLoop.hpp"

BEGIN_INANITY

Socket::Socket(ptr<EventLoop> eventLoop, uv_tcp_t* stream) :
	eventLoop(eventLoop), stream(stream)
{
	// запомнить себя
	stream->data = this;
	// создать дополнительную ссылку на сокет
	Reference();
}

Socket::~Socket()
{
	// к этому времени сокет уже закрыт. совсем.
	delete stream;
}

void Socket::CloseCallback(uv_handle_t* handle)
{
	// убрать ссылку, после того, как сокет закрылся
	((Socket*)handle->data)->Dereference();
}

void Socket::Close()
{
	uv_close((uv_handle_t*)stream, CloseCallback);
}

END_INANITY
