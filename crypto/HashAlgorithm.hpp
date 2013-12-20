#ifndef ___INANITY_CRYPTO_HASH_ALGORITHM_HPP___
#define ___INANITY_CRYPTO_HASH_ALGORITHM_HPP___

#include "crypto.hpp"
#include "../String.hpp"

BEGIN_INANITY_CRYPTO

class HashStream;

/// Класс алгоритма хеширования.
/** По сути - фабрика потоков хеширования. */
class HashAlgorithm : public Object
{
public:
	/// Gets an unique name of the algorithm.
	virtual String GetName() const = 0;
	/// Получить размер результирующего хеша.
	virtual size_t GetHashSize() const = 0;
	/// Создать поток для хеширования.
	virtual ptr<HashStream> CreateHashStream() = 0;
};

END_INANITY_CRYPTO

#endif
