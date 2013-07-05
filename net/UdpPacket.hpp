#ifndef ___INANITY_NET_UDP_PACKET_HPP___
#define ___INANITY_NET_UDP_PACKET_HPP___

#include "net.hpp"
#include "../Handler.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_NET

/// Абстрактный класс первого UDP-пакета.
class UdpPacket : public Object
{
public:
	/// Получить данные пакета.
	virtual ptr<File> GetData() const = 0;
	/// Создать сокет для получения дальнейших данных от этого клиента.
	virtual ptr<UdpSocket> CreateSocket() = 0;
};

END_INANITY_NET

#endif
