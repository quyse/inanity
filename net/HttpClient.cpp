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
		http_parser_url parsedUrl;
		if(http_parser_parse_url(url.c_str(), url.length(), 0, &parsedUrl))
			THROW_PRIMARY_EXCEPTION("Can't parse url");
		String host;
		if(parsedUrl.field_set & (1 << UF_HOST))
			host = url.substr(parsedUrl.field_data[UF_HOST].off, parsedUrl.field_data[UF_HOST].len);
		String path = "/";
		if(parsedUrl.field_set & (1 << UF_PATH))
			path = url.substr(parsedUrl.field_data[UF_PATH].off, parsedUrl.field_data[UF_PATH].len);
		int port = 80;
		if(parsedUrl.field_set & (1 << UF_PORT))
			port = parsedUrl.port;

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
