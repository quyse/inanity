#ifndef ___INANITY_CRYPTO_LAMPORT_SIGNATURE_ALGORITHM_HPP___
#define ___INANITY_CRYPTO_LAMPORT_SIGNATURE_ALGORITHM_HPP___

#include "SignatureAlgorithm.hpp"

BEGIN_INANITY_CRYPTO

class HashAlgorithm;

/// Класс алгоритма цифровой подписи Лампорта.
class LamportSignatureAlgorithm : public SignatureAlgorithm
{
private:
	ptr<HashAlgorithm> hashAlgorithm;
	size_t hashSize;

public:
	LamportSignatureAlgorithm(ptr<HashAlgorithm> hashAlgorithm);

	size_t GetDataSize() const;
	size_t GetPublicKeySize() const;
	size_t GetPrivateKeySize() const;
	size_t GetSignatureSize() const;

	void GenerateKeyPair(ptr<RandomAlgorithm> randomAlgorithm, void* privateKey, void* publicKey);
	void Sign(const void* data, const void* privateKey, void* signature);
	bool Verify(const void* data, const void* publicKey, const void* signature);
};

END_INANITY_CRYPTO

#endif
