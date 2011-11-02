#include "ClientSocket.hpp"
#include "EventLoop.hpp"
#include "MemoryFile.hpp"
#include <string.h>

ClientSocket::ClientSocket(ptr<EventLoop> eventLoop, uv_tcp_t* stream) :
	Socket(eventLoop, stream)
{
	uv_read_start((uv_stream_t*)stream, EventLoop::AllocCallback, ReadCallback);
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
		if(socket->streamToWriteInput)
			socket->streamToWriteInput->Flush();
	}
	else if(nread > 0)
	{
		// данные есть, выдать их наружу
		if(socket->streamToWriteInput)
			socket->streamToWriteInput->Write(buf.base, nread);
	}
	// если буфер предоставлен, освободить его
	if(buf.base)
		EventLoop::Free(buf);
}

void ClientSocket::ShutdownCallback(uv_shutdown_t* req, int status)
{
	ClientSocket* socket = (ClientSocket*)(Socket*)req->handle->data;
	delete req;
	socket->Close();
}

void ClientSocket::Shutdown()
{
	uv_shutdown_t* req = new uv_shutdown_t;
	uv_shutdown(req, (uv_stream_t*)stream, ShutdownCallback);
}

void ClientSocket::SetStreamToWriteInput(ptr<OutputStream> streamToWriteInput)
{
	this->streamToWriteInput = streamToWriteInput;
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
		// закрыть сокет
		socket->Close();
	}
	// в любом случае выполнить очистку
	delete (WriteRequest*)req->data;
}

void ClientSocket::SocketOutputStream::WriteFile(ptr<File> file)
{
	WriteRequest* request = new WriteRequest;
	request->req.data = request;
	request->file = file;
	uv_buf_t buf = uv_buf_init((char*)file->GetData(), file->GetSize());
	uv_write(&request->req, (uv_stream_t*)clientSocket->stream, &buf, 1, WriteCallback);
}

void ClientSocket::SocketOutputStream::Flush()
{
	clientSocket->Shutdown();
}
