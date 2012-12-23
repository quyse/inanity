#ifndef ___INANITY_NET_NET_HPP___
#define ___INANITY_NET_NET_HPP___

/* Общий файл для сетевой подсистемы. */

#include "../Object.hpp"

#define BEGIN_INANITY_NET BEGIN_INANITY namespace Net {
#define END_INANITY_NET } END_INANITY
#ifdef INANITY_LIB
BEGIN_INANITY_NET
END_INANITY_NET
using namespace Inanity::Net;
#endif

#endif
