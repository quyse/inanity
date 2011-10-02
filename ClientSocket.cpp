#include "ClientSocket.hpp"
#include "EventLoop.hpp"
#include "MemoryFile.hpp"
#include <string.h>

ClientSocket::ClientSocket(ptr<EventLoop> eventLoop, uv_tcp_t* stream) :
	Socket(eventLoop, stream)
{
}

void ClientSocket::ReadCallback(uv_stream_t* stream, ssize_t nread, uv_buf_t buf)
{
	ClientSocket* socket = (ClientSocket*)(Socket*)stream->data;
	if(nread < 0)
	{
		// ошибка или легальный конец данных
		// не разбираемся, в любом случае нужно закрыть сокет
		socket->Close();
		// сообщить о конце файла
		socket->streamToWriteInput->Flush();
	}
	else if(nread > 0)
	{
		// данные есть, выдать их наружу
		socket->streamToWriteInput->Write(buf.base, nread);
	}
	// если буфер предоставлен, освободить его
	if(buf.base)
		EventLoop::Free(buf);
}

void ClientSocket::ShutdownCallback(uv_shutdown_t* req, int status)
{
	delete req;
}

void ClientSocket::Shutdown()
{
	uv_shutdown_t* req = new uv_shutdown_t;
	uv_shutdown(req, (uv_stream_t*)stream, ShutdownCallback);
}

void ClientSocket::SetStreamToWriteInput(ptr<OutputStream> streamToWriteInput)
{
	bool newStream = !this->streamToWriteInput;
	this->streamToWriteInput = streamToWriteInput;
	if(newStream)
		uv_read_start((uv_stream_t*)stream, EventLoop::AllocCallback, ReadCallback);
}

ptr<OutputStream> ClientSocket::GetOutputStream()
{
	return NEW(SocketOutputStream(this));
}

ClientSocket::SocketOutputStream::SocketOutputStream(ptr<ClientSocket> clientSocket) :
	clientSocket(clientSocket)
{
}

void ClientSocket::SocketOutputStream::WriteCallback(uv_write_t* req, int status)
{
	ClientSocket* socket = (ClientSocket*)(Socket*)req->handle->data;
	if(status != 0)
	{
		// закрыть записывающую сторону сокета
		socket->Shutdown();
	}
	// в любом случае выполнить очистку
	for(int i = 0; i < req->bufcnt; ++i)
		delete [] (char*)req->bufs[i].base;
	delete req;
}

void ClientSocket::SocketOutputStream::Write(const void* data, size_t size)
{
	uv_write_t* req = new uv_write_t;
	uv_buf_t buf = uv_buf_init(new char[size], size);
	memcpy(buf.base, data, size);
	uv_write(req, (uv_stream_t*)clientSocket->stream, &buf, 1, WriteCallback);
}

void ClientSocket::SocketOutputStream::Flush()
{
	clientSocket->Shutdown();
}
