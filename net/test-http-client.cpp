#include "AsioService.hpp"
#include "TcpSocket.hpp"
#include "HttpClient.hpp"
#include "../inanity-base.hpp"
using namespace Inanity;
using namespace Inanity::Net;
#include <iostream>

class Processor : public SuccessHandler
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

	void OnSuccess()
	{
		std::cout << "Download completed successfully.\n";
	}

	void OnError(ptr<Exception> exception)
	{
		std::cout << "Download error:\n";
		exception->PrintStack(std::cout);
	}
};

int main(int argc, char** argv)
{
	try
	{
		argc--;
		argv++;

		if(argc != 2)
		{
			std::cout << "Arguments: <url> <output file>\n";
			return 0;
		}

		ptr<Service> service = NEW(AsioService());

		ptr<Processor> processor = NEW(Processor(service));

		HttpClient::Fetch(service, argv[0], processor, FolderFileSystem::GetNativeFileSystem()->SaveStream(argv[1]));

		ptr<Thread> thread = NEW(Thread(Thread::ThreadHandler::Bind<Processor>(processor, &Processor::Process)));

		thread->WaitEnd();
	}
	catch(Exception* exception)
	{
		std::cout << "Error in main.\n";
		MakePointer(exception)->PrintStack(std::cout);
	}

	return 0;
}
