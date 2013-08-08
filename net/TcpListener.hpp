#ifndef ___INANITY_NET_TCP_LISTENER_HPP___
#define ___INANITY_NET_TCP_LISTENER_HPP___

#include "net.hpp"

BEGIN_INANITY_NET

/// Server TCP listener abstract class.
/** Retains a reference to itself until closed. */
class TcpListener : public Object
{
public:
	/// Closes a listener.
	virtual void Close() = 0;
};

END_INANITY_NET

#endif
