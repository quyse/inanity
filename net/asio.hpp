#ifndef ___INANITY_NET_ASIO_HPP___
#define ___INANITY_NET_ASIO_HPP___

#include "net.hpp"

#ifdef ___INANITY_WINDOWS
#include "../platform/windows.hpp"
#endif

#define BOOST_ASIO_SEPARATE_COMPILATION
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>

#endif
