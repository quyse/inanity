#include "AsioService.hpp"
#include "TcpListener.hpp"
#include "TcpSocket.hpp"
#include "../inanity-base.hpp"
using namespace Inanity::Net;
#include <iostream>

class ClientWorker : public Object
{
private:
	int number;
	ptr<TcpSocket> socket;

public:
	ClientWorker(int number, ptr<TcpSocket> socket) : number(number), socket(socket) {}

	void Receive(const TcpSocket::ReceiveHandler::Result& result)
	{
		try
		{
			ptr<File> file = result.GetData();

			if(file)
			{
				std::cout << "Client " << number << " received:\n" << Strings::File2String(file) << "\n=======================\n";

				const char* response =
					"HTTP/1.1 200 OK\r\n"
					"Server: Inanity\r\n"
					"Content-Type: text/plain; charset=utf-8\r\n"
					"Connection: close\r\n"
					"Pragma: no-cache\r\n"
					"Cache-control: no-store\r\n"
					"\r\n"
					"The test text.\r\n";
				socket->Send(Strings::String2File(response));
				socket->End();
			}
			else
			{
				std::cout << "Client " << number << " received end of stream.\n";
				socket = 0;
			}
		}
		catch(Exception* exception)
		{
			socket = 0;
			std::cout << "Error receiving " << number << " client.\n";
			MakePointer(exception)->PrintStack(std::cout);
		}
	}
};

class Worker : public Object
{
private:
	int clientCount;
	ptr<Service> service;
	ptr<TcpListener> listener;

public:
	Worker(ptr<Service> service) : clientCount(0), service(service)
	{
		listener = service->ListenTcp(8080, Service::TcpSocketHandler::Bind<Worker>(this, &Worker::Accept));
	}

	void Accept(const Service::TcpSocketHandler::Result& result)
	{
		try
		{
			++clientCount;

			ptr<TcpSocket> socket = result.GetData();

			std::cout << "Accepted client " << clientCount << ".\n";

			ptr<ClientWorker> clientWorker = NEW(ClientWorker(clientCount, socket));
			socket->SetReceiveHandler(TcpSocket::ReceiveHandler::Bind(clientWorker, &ClientWorker::Receive));

			if(clientCount > 10)
			{
				listener->Close();
				listener = 0;
			}
		}
		catch(Exception* exception)
		{
			std::cout << "Error accepting " << clientCount << " connection.\n";
			MakePointer(exception)->PrintStack(std::cout);
		}
	}
};

int main()
{
	try
	{
		ptr<Service> service = NEW(AsioService());

		ptr<Worker> worker = NEW(Worker(service));

		service->Run();
	}
	catch(Exception* exception)
	{
		std::cout << "Error in main.\n";
		MakePointer(exception)->PrintStack(std::cout);
	}

	return 0;
}
