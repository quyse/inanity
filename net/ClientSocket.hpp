#ifndef ___INANITY_NET_CLIENT_SOCKET_HPP___
#define ___INANITY_NET_CLIENT_SOCKET_HPP___

#include "Socket.hpp"
#include "../OutputStream.hpp"

BEGIN_INANITY

class Exception;

END_INANITY

BEGIN_INANITY_NET

/// Класс клиентского сокета.
/** Олицетворяет собственно сокет на соединении. */
class ClientSocket : public Socket
{
private:
	ptr<OutputStream> streamToWriteInput;
	ptr<Exception> exception;

	class SocketOutputStream : public OutputStream
	{
	private:
		ptr<ClientSocket> clientSocket;

		struct WriteRequest
		{
			uv_write_t req;
			ptr<File> file;
		};

	public:
		SocketOutputStream(ptr<ClientSocket> clientSocket);

		static void WriteCallback(uv_write_t* req, int status);

		void WriteAsync(ptr<File> file);
		/// Закрыть сокет после завершения всех операций записи.
		void Flush();
	};

	static void ReadCallback(uv_stream_t* stream, ssize_t nread, uv_buf_t buf);
	static void ShutdownCallback(uv_shutdown_t* req, int status);

	void Shutdown();

public:
	ClientSocket(ptr<EventLoop> eventLoop, uv_tcp_t* stream);

	void SetStreamToWriteInput(ptr<OutputStream> streamToWriteInput);
	ptr<OutputStream> GetOutputStream();

	/// Получить исключение, если произошла ошибка. Если ошибки не было, то 0.
	ptr<Exception> GetError() const;
};

END_INANITY_NET

#endif
