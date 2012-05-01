#ifndef ___INANITY_CRYPTO_CNG_RANDOM_ALGORITHM_HPP___
#define ___INANITY_CRYPTO_CNG_RANDOM_ALGORITHM_HPP___

#include "RandomAlgorithm.hpp"

#ifndef ___INANITY_WINDOWS
#error CNG Random implemented on Windows only.
#endif

#include "../windows.hpp"
#include <bcrypt.h>

BEGIN_INANITY_CRYPTO

class CngRandomAlgorithm : public RandomAlgorithm
{
private:
	BCRYPT_ALG_HANDLE algorithm;

public:
	CngRandomAlgorithm();
	~CngRandomAlgorithm();

	void GenerateRandom(void* data, size_t size);
};

END_INANITY_CRYPTO

#endif
