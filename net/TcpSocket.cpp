#include "TcpSocket.hpp"
#include "../File.hpp"
#include "../OutputStream.hpp"

BEGIN_INANITY_NET

class TcpSocket::Stream : public OutputStream
{
private:
	ptr<TcpSocket> socket;

public:
	Stream(ptr<TcpSocket> socket) : socket(socket) {}

	void Write(ptr<File> file) override
	{
		socket->Send(file);
	}
};

ptr<OutputStream> TcpSocket::GetOutputStream()
{
	return NEW(Stream(this));
}

END_INANITY_NET
