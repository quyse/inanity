#ifndef ___INANITY_NET_ASIO_SERVICE_HPP___
#define ___INANITY_NET_ASIO_SERVICE_HPP___

#include "Service.hpp"
#include "asio.hpp"
#include <boost/scoped_ptr.hpp>
#include <botan/asio_stream.h>
#include <botan/certstor_system.h>
#include <botan/system_rng.h>
#include <botan/tls_policy.h>
#include <botan/tls_session_manager_memory.h>

BEGIN_INANITY_NET

/// Network service using Boost.Asio.
class AsioService : public Service
{
private:
	boost::asio::io_context ioContext;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work;
	boost::asio::ip::tcp::resolver tcpResolver;
	boost::asio::ip::udp::resolver udpResolver;

private:
	class TlsCredentialsManager : public Botan::Credentials_Manager
	{
	private:
		std::vector<std::unique_ptr<Botan::Certificate_Store>> certStores;

	public:
		TlsCredentialsManager();

		std::vector<Botan::Certificate_Store*> trusted_certificate_authorities(std::string const& type, std::string const& hostname) override;
	};

private:
	template <typename Socket>
	class ConnectTcpRequest;
	class ConnectUdpRequest;
	class TlsHandshakeHandler;

	std::unique_ptr<Botan::TLS::Context> CreateTlsContext(String const& host, int port);

public:
	AsioService();

	boost::asio::io_context& GetIoContext();

	static ptr<Exception> ConvertError(const boost::system::error_code& error);
	static ptr<Exception> ConvertError(const boost::system::system_error& error);

	//*** Service methods.
	void Run() override;
	void Stop() override;
	void Wait(std::function<void()> handler, int seconds) override;
	ptr<TcpListener> ListenTcp(int port, ptr<TcpSocketHandler> socketHandler) override;
	void ConnectTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler) override;
	ptr<UdpListener> ListenUdp(int port, ptr<UdpPacketHandler> receiveHandler) override;
	void ConnectUdp(const String& host, int port, ptr<UdpSocketHandler> socketHandler) override;
	void ConnectTlsTcp(const String& host, int port, ptr<TcpSocketHandler> socketHandler) override;
};

END_INANITY_NET

#endif
