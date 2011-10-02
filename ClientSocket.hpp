#ifndef ___INANITY_CLIENT_SOCKET_HPP___
#define ___INANITY_CLIENT_SOCKET_HPP___

#include "Socket.hpp"
#include "OutputStream.hpp"

BEGIN_INANITY

/// Класс клиентского сокета.
/** Олицетворяет собственно сокет на соединении. */
class ClientSocket : public Socket
{
private:
	ptr<OutputStream> streamToWriteInput;

	class SocketOutputStream : public OutputStream
	{
	private:
		ptr<ClientSocket> clientSocket;

	public:
		SocketOutputStream(ptr<ClientSocket> clientSocket);

		static void WriteCallback(uv_write_t* req, int status);

		void Write(const void* data, size_t size);
		/// Закрыть записывающую сторону сокета.
		void Flush();
	};

	static void ReadCallback(uv_stream_t* stream, ssize_t nread, uv_buf_t buf);
	static void ShutdownCallback(uv_shutdown_t* req, int status);

	void Shutdown();

public:
	ClientSocket(ptr<EventLoop> eventLoop, uv_tcp_t* stream);

	void SetStreamToWriteInput(ptr<OutputStream> streamToWriteInput);
	ptr<OutputStream> GetOutputStream();
};

END_INANITY

#endif
