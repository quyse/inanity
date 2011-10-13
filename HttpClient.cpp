#include "HttpClient.hpp"
#include "EventLoop.hpp"
#include "OutputStream.hpp"
#include "ClientSocket.hpp"
#include "HttpResponseStream.hpp"
#include "Exception.hpp"
#include <cstdlib>
#include <sstream>

// простой URL-парсер, выделяющий хост, порт и путь
static void parseUrl(const String& url, String& host, int& port, String& path)
{
	try
	{
		// проверить, что URL начинается с http://
		static const char* protocol = "http://";
		static const size_t protocolLength = 7;
		if(url.length() < protocolLength || url.compare(0, protocolLength, protocol) != 0)
			THROW_PRIMARY_EXCEPTION("http:// not found");

		// найти слеш, отделяющий хост и порт от пути
		size_t pathSlashPos = url.find('/', protocolLength);
		if(pathSlashPos == url.npos)
			THROW_PRIMARY_EXCEPTION("Path begin slash not found");
		path = url.substr(pathSlashPos);

		// найти двоеточие, обозначающее порт
		size_t portColonPos = url.find(':', protocolLength);
		if(portColonPos == url.npos || portColonPos >= pathSlashPos)
		{
			// если порт не указан
			host = url.substr(protocolLength, pathSlashPos - protocolLength);
			port = 80;
		}
		else
		{
			// если порт указан
			host = url.substr(protocolLength, portColonPos - protocolLength);
			port = atoi(url.substr(portColonPos + 1, pathSlashPos - portColonPos - 1).c_str());
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("URL must be of form http://<host>[:<port>]/[query]", exception);
	}
}

HttpClient::ConnectHandler::ConnectHandler(const String& host, int port, const String& path, ptr<OutputStream> outputStream) :
	outputStream(outputStream)
{
	std::ostringstream request;
	request << "GET " << path << " HTTP/1.1\r\n";
	request << "Connection: close\r\n";
	request << "Host: " << host;
	if(port != 80)
		request << ":" << port;
	request << "\r\n";
	request << "User-Agent: Inanity/1.0\r\n";
	request << "\r\n";
	this->request = request.str();
}

void HttpClient::ConnectHandler::OnEvent(ptr<ClientSocket> socket)
{
	if(!socket)
	{
		outputStream->Flush();
		return;
	}

	// указать, куда записывать ответ
	socket->SetStreamToWriteInput(NEW(HttpResponseStream(outputStream)));
	// отправить HTTP-запрос
	socket->GetOutputStream()->Write(request.c_str(), request.length());
}

void HttpClient::Fetch(ptr<EventLoop> eventLoop, const String& url, ptr<OutputStream> outputStream)
{
	try
	{
		String host, path;
		int port;
		parseUrl(url, host, port, path);
		eventLoop->Connect(host, port, NEW(ConnectHandler(host, port, path, outputStream)));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't fetch http", exception);
	}
}
