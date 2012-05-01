#ifndef ___INANITY_CRYPTO_RANDOM_ALGORITHM_HPP___
#define ___INANITY_CRYPTO_RANDOM_ALGORITHM_HPP___

#include "crypto.hpp"

BEGIN_INANITY_CRYPTO

/// Класс алгоритма генератора случайных чисел.
class RandomAlgorithm : public Object
{
public:
	/// Сгенерировать заданное количество случайных данных.
	virtual void GenerateRandom(void* data, size_t size) = 0;
};

END_INANITY_CRYPTO

#endif
