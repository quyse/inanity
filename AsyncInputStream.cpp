#include "AsyncInputStream.hpp"
#include "Exception.hpp"

AsyncInputStream::AsyncInputStream(ptr<Handler> handler) : handler(handler)
{
}
