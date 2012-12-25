#include "Socket.hpp"
#include "EventLoop.hpp"

Socket::Socket(ptr<EventLoop> eventLoop) :
	eventLoop(eventLoop), socket(eventLoop->GetIoService())
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
