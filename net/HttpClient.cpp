#include "HttpClient.hpp"
#include "Service.hpp"
#include "TcpSocket.hpp"
#include "HttpResponseStream.hpp"
#include "../File.hpp"
#include "../OutputStream.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include <sstream>

BEGIN_INANITY_NET

class HttpClientRequest : public Object
{
private:
	ptr<File> requestFile;
	ptr<SuccessHandler> handler;
	ptr<HttpResponseStream> outputStream;

public:
	HttpClientRequest(ptr<Service> service, const String& url, ptr<SuccessHandler> handler, ptr<OutputStream> outputStream)
	: handler(handler), outputStream(NEW(HttpResponseStream(outputStream)))
	{
		// разобрать URL
		String host, path;
		int port;
		ParseUrl(url, host, port, path);

		// сформировать запрос
		std::ostringstream request;
		request << "GET " << path << " HTTP/1.1\r\n";
		request << "Connection: close\r\n";
		request << "Host: " << host;
		if(port != 80)
			request << ":" << port;
		request << "\r\n";
		request << "User-Agent: Inanity HttpClient/2.0\r\n";
		request << "\r\n";

		requestFile = Strings::String2File(request.str());

		service->ConnectTcp(host, port, Service::TcpSocketHandler::Bind(MakePointer(this), &HttpClientRequest::OnConnect));
	}

private:
	void OnConnect(const Service::TcpSocketHandler::Result& result)
	{
		ptr<HttpClientRequest> self = this;

		ptr<TcpSocket> socket;
		try
		{
			socket = result.GetData();

			// установить принимающий обработчик
			socket->SetReceiveHandler(TcpSocket::ReceiveHandler::Bind(MakePointer(this), &HttpClientRequest::OnReceive));

			// отправить HTTP-запрос
			socket->Send(requestFile);
			requestFile = 0;
			// закрыть передающую сторону
			socket->End();
		}
		catch(Exception* exception)
		{
			if(socket)
				socket->Close();
			handler->FireError(exception);
		}
	}

	void OnReceive(const TcpSocket::ReceiveHandler::Result& result)
	{
		try
		{
			ptr<File> data = result.GetData();

			if(data)
				// пришли данные
				outputStream->WriteFile(data);
			else
			{
				// корректный конец данных
				outputStream->End();
				if(outputStream->IsCompleted())
					handler->FireSuccess();
				else
					THROW_PRIMARY_EXCEPTION("HTTP response is not completed");
			}
		}
		catch(Exception* exception)
		{
			// ошибка получения
			handler->FireError(exception);
		}
	}

	// простой URL-парсер, выделяющий хост, порт и путь
	static void ParseUrl(const String& url, String& host, int& port, String& path)
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
			THROW_SECONDARY_EXCEPTION("URL must be of form http://<host>[:<port>]/[<query>]", exception);
		}
	}
};

void HttpClient::Fetch(ptr<Service> service, const String& url, ptr<SuccessHandler> handler, ptr<OutputStream> outputStream)
{
	try
	{
		NEW(HttpClientRequest(service, url, handler, outputStream));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't fetch http", exception);
	}
}

END_INANITY_NET
