#include "CngRandomAlgorithm.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_CRYPTO

#define NT_SUCCESS(Status) \
	(((NTSTATUS)(Status)) >= 0)

CngRandomAlgorithm::CngRandomAlgorithm()
{
	try
	{
		if(!NT_SUCCESS(BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_RNG_ALGORITHM, NULL, 0)))
			THROW("Can't open algorithm provider");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create CNG Random Algorithm", exception);
	}
}

CngRandomAlgorithm::~CngRandomAlgorithm()
{
	BCryptCloseAlgorithmProvider(algorithm, 0);
}

void CngRandomAlgorithm::GenerateRandom(void* data, size_t size)
{
	THROW_ASSERT(size == (ULONG)size);
	if(!NT_SUCCESS(BCryptGenRandom(algorithm, (PUCHAR)data, (ULONG)size, 0)))
		THROW("Can't generate random data");
}

END_INANITY_CRYPTO
