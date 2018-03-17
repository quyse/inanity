#ifndef ___INANITY_NET_EMS_SERVICE_HPP___
#define ___INANITY_NET_EMS_SERVICE_HPP___

#include "Service.hpp"
#include <unordered_map>

BEGIN_INANITY_NET

class EmsTcpSocket;

/// Network service using emscripten's async network APIs.
class EmsService : public Service
{
private:
	class ConnectTcpRequest;

public:
	EmsService();

	//*** Service's methods.
	void Run() override;
	void Stop() override;
	void Wait(std::function<void()> handler, int seconds) override;
	void ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler) override;

	void RegisterTcpSocket(int fd, EmsTcpSocket* tcpSocket);
	void UnregisterTcpSocket(int fd);

private:
	void OpenCallback(int fd);
	void MessageCallback(int fd);
	void CloseCallback(int fd);

	static void StaticOpenCallback(int fd, void* userData);
	static void StaticMessageCallback(int fd, void* userData);
	static void StaticCloseCallback(int fd, void* userData);

	std::unordered_map<int, EmsTcpSocket*> tcpSockets;
};

END_INANITY_NET

#endif
