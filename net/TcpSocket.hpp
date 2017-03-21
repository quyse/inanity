#ifndef ___INANITY_NET_TCP_SOCKET_HPP___
#define ___INANITY_NET_TCP_SOCKET_HPP___

#include "net.hpp"
#include "../Handler.hpp"

BEGIN_INANITY

class File;
class OutputStream;

END_INANITY

BEGIN_INANITY_NET

/// TCP socket abstract class.
/** Retains a reference to itself until receiving side is closed.
Sending side is not retains a reference (because anyway you need
a reference if you want to send something).
If you got a correct receiving data end, or receiving error,
you don't need to explicitly close socket: in this case it releases
automatically. You need to close socket if you want to stop
receiving intentionally (for example, if you got sending error).
*/
class TcpSocket : public Object
{
public:
	typedef SuccessHandler SendHandler;
	/// Receive data handler.
	/** If the socket is closed correctly, handler will called last and once
	with zero file pointer. */
	typedef DataHandler<ptr<File> > ReceiveHandler;

	/// Socket as output stream.
	class Stream;

public:
	/// Write data to the socket.
	/** Data goes into queue, and will be sent in order. */
	virtual void Send(ptr<File> file, ptr<SendHandler> sendHandler = 0) = 0;
	/// Close sending side of the socket.
	/** Socket will be closed only when all enqueued data will be sent. */
	virtual void End() = 0;
	/// Set receive data handler.
	virtual void SetReceiveHandler(ptr<ReceiveHandler> receiveHandler) = 0;
	/// Set NoDelay option.
	virtual void SetNoDelay(bool noDelay) = 0;
	/// Close receiving data side.
	/** Releases references to itself and all handlers. */
	virtual void Close() = 0;

	ptr<OutputStream> GetOutputStream();
};

END_INANITY_NET

#endif
