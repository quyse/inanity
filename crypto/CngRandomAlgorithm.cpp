#include "CngRandomAlgorithm.hpp"
#include "../Exception.hpp"

#define NT_SUCCESS(Status) \
	(((NTSTATUS)(Status)) >= 0)

CngRandomAlgorithm::CngRandomAlgorithm()
{
	try
	{
		if(!NT_SUCCESS(BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_RNG_ALGORITHM, NULL, 0)))
			THROW_PRIMARY_EXCEPTION("Can't open algorithm provider");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create CNG Random Algorithm", exception);
	}
}

CngRandomAlgorithm::~CngRandomAlgorithm()
{
	BCryptCloseAlgorithmProvider(algorithm, 0);
}

void CngRandomAlgorithm::GenerateRandom(void* data, size_t size)
{
	if(!NT_SUCCESS(BCryptGenRandom(algorithm, (PUCHAR)data, size, 0)))
		THROW_PRIMARY_EXCEPTION("Can't generate random data");
}
