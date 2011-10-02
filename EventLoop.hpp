#ifndef ___INANITY_EVENT_LOOP_HPP___
#define ___INANITY_EVENT_LOOP_HPP___

#include "Object.hpp"
#include "ServerSocket.hpp"
#include <uv.h>

/*
 * Файл содержит класс цикла обработки асинхронных событий,
 * основанного на библиотеке libuv.
 */

BEGIN_INANITY

class EventLoop : public Object
{
	friend class ServerSocket;
private:
	uv_loop_t* loop;

public:
	EventLoop();
	~EventLoop();

	static uv_buf_t AllocCallback(uv_handle_t* handle, size_t size);
	static void Free(uv_buf_t buf);

	ptr<ServerSocket> Listen(int port, ptr<ServerSocket::Handler> handler);
};

END_INANITY

#endif
