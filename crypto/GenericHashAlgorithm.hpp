#ifndef ___INANITY_CRYPTO_GENERIC_HASH_ALGORITHM_HPP___
#define ___INANITY_CRYPTO_GENERIC_HASH_ALGORITHM_HPP___

#include "HashAlgorithm.hpp"

BEGIN_INANITY_CRYPTO

class HashStream;

/// Обобщённый класс хеш-алгоритма.
template <typename AlgorithmHashStream>
class GenericHashAlgorithm : public HashAlgorithm
{
public:
	size_t GetHashSize() const
	{
		return AlgorithmHashStream::hashSize;
	}

	/// Создать поток для хеширования.
	ptr<HashStream> CreateHashStream()
	{
		return NEW(AlgorithmHashStream());
	}
};

END_INANITY_CRYPTO

#endif
