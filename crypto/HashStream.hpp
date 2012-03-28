#ifndef ___INANITY_HASH_STREAM_HPP___
#define ___INANITY_HASH_STREAM_HPP___

#include "crypto.hpp"
#include "../OutputStream.hpp"
#include "../String.hpp"

BEGIN_INANITY_CRYPTO

/// Класс хеширующего выходного потока.
/** Просто вычисляет хеш приходящих данных по какому-либо алгоритму. */
class HashStream : public OutputStream
{
public:
	/// Получить размер хеша в байтах.
	virtual size_t GetHashSize() const = 0;
	/// Получить хеш.
	/** Функцию можно вызывать только после вызова Flush().
	Допустимо вызывать её многократно.
	\param data Буфер для хеша достаточного размера. */
	virtual void GetHash(void* data) const = 0;

	/// Получить хеш как строку.
	/** Получает хеш в виде 16-ричного представления. */
	String GetHashString() const;
};

END_INANITY_CRYPTO

#endif
