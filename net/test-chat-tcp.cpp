#include "AsioService.hpp"
#include "TcpListener.hpp"
#include "TcpSocket.hpp"
#include "../inanity-base.hpp"
using namespace Inanity;
using namespace Inanity::Net;
#include <iostream>
#include <vector>

class ChatBase : public Object
{
public:
	virtual bool Command(const String& command) = 0;
};

class ChatClient : public ChatBase
{
private:
	ptr<TcpSocket> socket;
	CriticalSection cs;

	void Connected(const Service::TcpSocketHandler::Result& result)
	{
		try
		{
			socket = result.GetData();

			socket->SetReceiveHandler(TcpSocket::ReceiveHandler::Bind<ChatClient>(this, &ChatClient::Received));
		}
		catch(Exception* exception)
		{
			std::cout << "Can't connect to chat server\n";
			MakePointer(exception)->PrintStack(std::cout);
		}
	}

	void Received(const TcpSocket::ReceiveHandler::Result& result)
	{
		CriticalCode cc(cs);
		try
		{
			ptr<File> file = result.GetData();
			if(!file)
			{
				std::cout << "Server closed connection\n";
				Close();
				return;
			}

			std::cout << Strings::File2String(file);
		}
		catch(Exception* exception)
		{
			std::cout << "Can't receive data\n";
			MakePointer(exception)->PrintStack(std::cout);
			Close();
		}
	}

	void Close()
	{
		if(socket)
		{
			socket->Close();
			socket = 0;
		}
	}

public:
	ChatClient(ptr<Service> service, const String& host)
	{
		service->ConnectTcp(host, 8080, Service::TcpSocketHandler::Bind<ChatClient>(this, &ChatClient::Connected));
	}

	bool Command(const String& command)
	{
		CriticalCode cc(cs);

		if(!socket)
		{
			std::cout << "Already exited.\n";
			return false;
		}

		if(command.length() && command[0] == '@')
		{
			if(command == "@exit")
			{
				Close();
				return false;
			}
			else
			{
				std::cout << "Unknown chat client command: " << command << ".\n";
				return true;
			}
		}

		socket->Send(Strings::String2File(command));
		return true;
	}
};

class ChatServer : public ChatBase
{
private:
	ptr<TcpListener> listener;
	CriticalSection cs;

	class Client : public Object
	{
	private:
		ptr<ChatServer> server;
		ptr<TcpSocket> socket;

		void Received(const TcpSocket::ReceiveHandler::Result& result)
		{
			try
			{
				ptr<File> file = result.GetData();
				if(!file)
				{
					std::cout << "Client " << this << " disconnected.\n";
					Close();
					return;
				}
				server->Received(this, file);
			}
			catch(Exception* exception)
			{
				std::cout << "Can't receive data from client\n";
				MakePointer(exception)->PrintStack(std::cout);
				Close();
			}
		}

	public:
		Client(ptr<ChatServer> server, ptr<TcpSocket> socket)
		: server(server), socket(socket)
		{
			socket->SetReceiveHandler(TcpSocket::ReceiveHandler::Bind<Client>(this, &Client::Received));
		}

		void Send(ptr<File> file)
		{
			socket->Send(file);
		}

		void Close()
		{
			if(socket)
			{
				socket->Close();
				socket = 0;
				server->Closed(this);
			}
		}
	};

	std::vector<ptr<Client> > clients;

	void Accepted(const Service::TcpSocketHandler::Result& result)
	{
		CriticalCode cc(cs);
		try
		{
			ptr<Client> client = NEW(Client(this, result.GetData()));
			clients.push_back(client);
			std::cout << "Client " << &*client << " accepted.\n";
			PrintClients();
		}
		catch(Exception* exception)
		{
			std::cout << "Can't accept client\n";
			MakePointer(exception)->PrintStack(std::cout);
		}
	}

	void Received(ptr<Client> client, ptr<File> file)
	{
		std::cout << "Client " << &*client << " wrote:\n" << Strings::File2String(file) << "\n";
		for(size_t i = 0; i < clients.size(); ++i)
			clients[i]->Send(file);
	}

	void Closed(ptr<Client> client)
	{
		clients.erase(std::find(clients.begin(), clients.end(), client));
		std::cout << "Client " << &*client << " closed.\n";
		PrintClients();
	}

	void PrintClients()
	{
		std::cout << "Clients count: " << clients.size() << ".\n";
	}

public:
	ChatServer(ptr<Service> service)
	{
		listener = service->ListenTcp(8080, Service::TcpSocketHandler::Bind<ChatServer>(this, &ChatServer::Accepted));
	}

	bool Command(const String& command)
	{
		if(command == "@exit")
		{
			listener->Close();
			listener = 0;
			std::vector<ptr<Client> > clientsCopy(clients);
			for(size_t i = 0; i < clientsCopy.size(); ++i)
				clientsCopy[i]->Close();
			return false;
		}
		else
		{
			std::cout << "Unknown chat server command: " << command << ".\n";
			return true;
		}
	}
};

class Processor : public Object
{
private:
	ptr<Service> service;

public:
	Processor(ptr<Service> service) : service(service) {}

	void Process(const Thread::ThreadHandler::Result& result)
	{
		service->Run();
		std::cout << "Processor exiting.\n";
	}
};

int main()
{
	try
	{
		ptr<Service> service = NEW(AsioService());

		std::cout << "Server or client? (s/c): ";
		String sc;
		std::cin >> sc;

		ptr<ChatBase> chat;

		if(sc == "s")
		{
			chat = NEW(ChatServer(service));
		}
		else if(sc == "c")
		{
			std::cout << "Host: ";
			String host;
			std::cin >> host;
			chat = NEW(ChatClient(service, host));
		}
		else
			THROW_PRIMARY_EXCEPTION("Wrong answer");

		ptr<Thread> thread = NEW(Thread(Thread::ThreadHandler::Bind<Processor>(NEW(Processor(service)), &Processor::Process)));

		for(;;)
		{
			std::string command;
			std::cin >> command;
			if(!chat->Command(command))
				break;
		}

		thread->WaitEnd();
	}
	catch(Exception* exception)
	{
		std::cout << "Error in main.\n";
		MakePointer(exception)->PrintStack(std::cout);
	}

	return 0;
}
