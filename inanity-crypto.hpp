#ifndef ___INANITY_INANITY_CRYPTO_HPP___
#define ___INANITY_INANITY_CRYPTO_HPP___

#include "crypto/HashAlgorithm.hpp"
#include "crypto/GenericHashAlgorithm.hpp"
#include "crypto/HashStream.hpp"
#include "crypto/WhirlpoolStream.hpp"
#include "crypto/SignatureAlgorithm.hpp"
#include "crypto/LamportSignatureAlgorithm.hpp"
#include "crypto/RandomAlgorithm.hpp"
#include "crypto/StreamHasher.hpp"
#include "crypto/StreamSigner.hpp"
#ifdef ___INANITY_WINDOWS
#include "crypto/CngRandomAlgorithm.hpp"
#endif

#endif
