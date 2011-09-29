#include "AsyncOutputStream.hpp"

AsyncOutputStream::AsyncOutputStream(ptr<Handler> handler) : handler(handler)
{
}
