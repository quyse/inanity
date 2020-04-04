#ifndef BOTAN_BUILD_CONFIG_H_
#define BOTAN_BUILD_CONFIG_H_

/*
* This file was automatically generated running
* 'configure.py --with-build-dir=../build --module-policy=modern --link-method=copy --cpu=x86_64 --with-zlib --with-boost --enable-modules=certstor_system --disable-modules=certstor_sql,win32_stats --os=macos --cc=clang --enable-modules=certstor_system_macos'
*
* Target
*  - Compiler: clang++ -fstack-protector -m64 -pthread -stdlib=libc++ -std=c++11 -D_REENTRANT -O3
*  - Arch: x86_64
*  - OS: macos
*/

#define BOTAN_VERSION_MAJOR 2
#define BOTAN_VERSION_MINOR 13
#define BOTAN_VERSION_PATCH 0
#define BOTAN_VERSION_DATESTAMP 0

#define BOTAN_VERSION_RELEASE_TYPE "unreleased"

#define BOTAN_VERSION_VC_REVISION "git:ed360ab268544fd801b8ea68ff08b07610680052"

#define BOTAN_DISTRIBUTION_INFO "unspecified"

/* How many bits per limb in a BigInt */
#define BOTAN_MP_WORD_BITS 64


#define BOTAN_INSTALL_PREFIX R"(/usr/local)"
#define BOTAN_INSTALL_HEADER_DIR R"(include/botan-2)"
#define BOTAN_INSTALL_LIB_DIR R"(/usr/local/lib)"
#define BOTAN_LIB_LINK "-lboost_system -lz -framework CoreFoundation -framework Security"
#define BOTAN_LINK_FLAGS "-fstack-protector -m64 -pthread -stdlib=libc++"

#define BOTAN_SYSTEM_CERT_BUNDLE "/etc/ssl/certs/ca-certificates.crt"

#ifndef BOTAN_DLL
  #define BOTAN_DLL __attribute__((visibility("default")))
#endif

/* Target identification and feature test macros */

#define BOTAN_TARGET_OS_IS_MACOS

#define BOTAN_TARGET_OS_HAS_APPLE_KEYCHAIN
#define BOTAN_TARGET_OS_HAS_ARC4RANDOM
#define BOTAN_TARGET_OS_HAS_CLOCK_GETTIME
#define BOTAN_TARGET_OS_HAS_COMMONCRYPTO
#define BOTAN_TARGET_OS_HAS_DEV_RANDOM
#define BOTAN_TARGET_OS_HAS_FILESYSTEM
#define BOTAN_TARGET_OS_HAS_GETENTROPY
#define BOTAN_TARGET_OS_HAS_POSIX1
#define BOTAN_TARGET_OS_HAS_POSIX_MLOCK
#define BOTAN_TARGET_OS_HAS_SOCKETS
#define BOTAN_TARGET_OS_HAS_THREAD_LOCAL
#define BOTAN_TARGET_OS_HAS_THREADS


#define BOTAN_BUILD_COMPILER_IS_CLANG




#define BOTAN_TARGET_ARCH_IS_X86_64
#define BOTAN_TARGET_CPU_IS_LITTLE_ENDIAN
#define BOTAN_TARGET_CPU_IS_X86_FAMILY
#define BOTAN_TARGET_CPU_HAS_NATIVE_64BIT

#define BOTAN_TARGET_SUPPORTS_AESNI
#define BOTAN_TARGET_SUPPORTS_AVX2
#define BOTAN_TARGET_SUPPORTS_BMI2
#define BOTAN_TARGET_SUPPORTS_RDRAND
#define BOTAN_TARGET_SUPPORTS_RDSEED
#define BOTAN_TARGET_SUPPORTS_SHA
#define BOTAN_TARGET_SUPPORTS_SSE2
#define BOTAN_TARGET_SUPPORTS_SSE41
#define BOTAN_TARGET_SUPPORTS_SSE42
#define BOTAN_TARGET_SUPPORTS_SSSE3






/*
* Module availability definitions
*/
#define BOTAN_HAS_AEAD_CHACHA20_POLY1305 20180807
#define BOTAN_HAS_AEAD_GCM 20131128
#define BOTAN_HAS_AEAD_MODES 20131128
#define BOTAN_HAS_AEAD_OCB 20131128
#define BOTAN_HAS_AES 20131128
#define BOTAN_HAS_AES_NI 20131128
#define BOTAN_HAS_AES_VPERM 20190901
#define BOTAN_HAS_ASN1 20171109
#define BOTAN_HAS_AUTO_RNG 20161126
#define BOTAN_HAS_AUTO_SEEDING_RNG 20160821
#define BOTAN_HAS_BASE64_CODEC 20131128
#define BOTAN_HAS_BCRYPT 20131128
#define BOTAN_HAS_BIGINT 20131128
#define BOTAN_HAS_BIGINT_MP 20151225
#define BOTAN_HAS_BLAKE2B 20130131
#define BOTAN_HAS_BLOCK_CIPHER 20131128
#define BOTAN_HAS_BLOWFISH 20180718
#define BOTAN_HAS_BOOST_ASIO 20131228
#define BOTAN_HAS_CERTSTOR_MACOS 20190207
#define BOTAN_HAS_CERTSTOR_SYSTEM 20190411
#define BOTAN_HAS_CHACHA 20180807
#define BOTAN_HAS_CHACHA_AVX2 20180418
#define BOTAN_HAS_CHACHA_SIMD32 20181104
#define BOTAN_HAS_CIPHER_MODES 20180124
#define BOTAN_HAS_CIPHER_MODE_PADDING 20131128
#define BOTAN_HAS_CMAC 20131128
#define BOTAN_HAS_COMPRESSION 20141117
#define BOTAN_HAS_CPUID 20170917
#define BOTAN_HAS_CTR_BE 20131128
#define BOTAN_HAS_CURVE_25519 20170621
#define BOTAN_HAS_DIFFIE_HELLMAN 20131128
#define BOTAN_HAS_DL_GROUP 20131128
#define BOTAN_HAS_DL_PUBLIC_KEY_FAMILY 20131128
#define BOTAN_HAS_ECC_GROUP 20170225
#define BOTAN_HAS_ECC_KEY 20190801
#define BOTAN_HAS_ECC_PUBLIC_KEY_CRYPTO 20131128
#define BOTAN_HAS_ECDH 20131128
#define BOTAN_HAS_ECDSA 20131128
#define BOTAN_HAS_EC_CURVE_GFP 20131128
#define BOTAN_HAS_ED25519 20170607
#define BOTAN_HAS_EME_OAEP 20180305
#define BOTAN_HAS_EME_PKCS1 20190426
#define BOTAN_HAS_EME_PKCS1v15 20131128
#define BOTAN_HAS_EMSA1 20131128
#define BOTAN_HAS_EMSA_PKCS1 20140118
#define BOTAN_HAS_EMSA_PSSR 20131128
#define BOTAN_HAS_ENTROPY_SOURCE 20151120
#define BOTAN_HAS_ENTROPY_SRC_DEV_RANDOM 20131128
#define BOTAN_HAS_ENTROPY_SRC_RDRAND 20131128
#define BOTAN_HAS_ENTROPY_SRC_RDSEED 20151218
#define BOTAN_HAS_FFI 20191214
#define BOTAN_HAS_GCM_CLMUL_CPU 20131227
#define BOTAN_HAS_GCM_CLMUL_SSSE3 20171020
#define BOTAN_HAS_HASH 20180112
#define BOTAN_HAS_HASH_ID 20131128
#define BOTAN_HAS_HEX_CODEC 20131128
#define BOTAN_HAS_HKDF 20170927
#define BOTAN_HAS_HMAC 20131128
#define BOTAN_HAS_HMAC_DRBG 20140319
#define BOTAN_HAS_HTTP_UTIL 20171003
#define BOTAN_HAS_KDF2 20131128
#define BOTAN_HAS_KDF_BASE 20131128
#define BOTAN_HAS_KECCAK 20131128
#define BOTAN_HAS_KEYPAIR_TESTING 20131128
#define BOTAN_HAS_LOCKING_ALLOCATOR 20131128
#define BOTAN_HAS_MAC 20150626
#define BOTAN_HAS_MDX_HASH_FUNCTION 20131128
#define BOTAN_HAS_MEM_POOL 20180309
#define BOTAN_HAS_MGF1 20140118
#define BOTAN_HAS_MODES 20150626
#define BOTAN_HAS_MODE_CBC 20131128
#define BOTAN_HAS_NEWHOPE 20161018
#define BOTAN_HAS_NUMBERTHEORY 20131128
#define BOTAN_HAS_OCSP 20161118
#define BOTAN_HAS_PBKDF 20180902
#define BOTAN_HAS_PBKDF2 20180902
#define BOTAN_HAS_PEM_CODEC 20131128
#define BOTAN_HAS_PKCS5_PBES2 20141119
#define BOTAN_HAS_PK_PADDING 20131128
#define BOTAN_HAS_POLY1305 20141227
#define BOTAN_HAS_POLY_DBL 20170927
#define BOTAN_HAS_PUBLIC_KEY_CRYPTO 20131128
#define BOTAN_HAS_RDRAND_RNG 20160619
#define BOTAN_HAS_RFC6979_GENERATOR 20140321
#define BOTAN_HAS_RSA 20160730
#define BOTAN_HAS_SERPENT 20131128
#define BOTAN_HAS_SERPENT_AVX2 20180824
#define BOTAN_HAS_SERPENT_SIMD 20160903
#define BOTAN_HAS_SHA1 20131128
#define BOTAN_HAS_SHA1_SSE2 20160803
#define BOTAN_HAS_SHA1_X86_SHA_NI 20170518
#define BOTAN_HAS_SHA2_32 20131128
#define BOTAN_HAS_SHA2_32_X86 20170518
#define BOTAN_HAS_SHA2_32_X86_BMI2 20180526
#define BOTAN_HAS_SHA2_64 20131128
#define BOTAN_HAS_SHA2_64_BMI2 20190117
#define BOTAN_HAS_SHA3 20161018
#define BOTAN_HAS_SHA3_BMI2 20190117
#define BOTAN_HAS_SHAKE_CIPHER 20161018
#define BOTAN_HAS_SIMD_32 20131128
#define BOTAN_HAS_SIMD_AVX2 20180824
#define BOTAN_HAS_SIPHASH 20150110
#define BOTAN_HAS_SKEIN_512 20131128
#define BOTAN_HAS_SOCKETS 20171216
#define BOTAN_HAS_STATEFUL_RNG 20160819
#define BOTAN_HAS_STREAM_CIPHER 20131128
#define BOTAN_HAS_SYSTEM_RNG 20141202
#define BOTAN_HAS_THREEFISH_512 20131224
#define BOTAN_HAS_THREEFISH_512_AVX2 20160903
#define BOTAN_HAS_TLS 20191210
#define BOTAN_HAS_TLS_SESSION_MANAGER_SQL_DB 20141219
#define BOTAN_HAS_TLS_V10_PRF 20131128
#define BOTAN_HAS_TLS_V12_PRF 20131128
#define BOTAN_HAS_UTIL_FUNCTIONS 20180903
#define BOTAN_HAS_X25519 20180910
#define BOTAN_HAS_X509 20180911
#define BOTAN_HAS_X509_CERTIFICATES 20151023
#define BOTAN_HAS_ZLIB 20160412


/*
* Local/misc configuration options (if any) follow
*/


/*
* Things you can edit (but probably shouldn't)
*/

#if !defined(BOTAN_DEPRECATED_PUBLIC_MEMBER_VARIABLES)

  #if defined(BOTAN_NO_DEPRECATED)
     #define BOTAN_DEPRECATED_PUBLIC_MEMBER_VARIABLES private
  #else
     #define BOTAN_DEPRECATED_PUBLIC_MEMBER_VARIABLES public
  #endif

#endif

/* How much to allocate for a buffer of no particular size */
#define BOTAN_DEFAULT_BUFFER_SIZE 1024

/*
* Total maximum amount of RAM (in KiB) we will lock into memory, even
* if the OS would let us lock more
*/
#define BOTAN_MLOCK_ALLOCATOR_MAX_LOCKED_KB 512

/*
* If BOTAN_MEM_POOL_USE_MMU_PROTECTIONS is defined, the Memory_Pool
* class used for mlock'ed memory will use OS calls to set page
* permissions so as to prohibit access to pages on the free list, then
* enable read/write access when the page is set to be used. This will
* turn (some) use after free bugs into a crash.
*
* The additional syscalls have a substantial performance impact, which
* is why this option is not enabled by default.
*/
#if defined(BOTAN_HAS_VALGRIND) || defined(BOTAN_ENABLE_DEBUG_ASSERTS)
   #define BOTAN_MEM_POOL_USE_MMU_PROTECTIONS
#endif

/*
* If enabled uses memset via volatile function pointer to zero memory,
* otherwise does a byte at a time write via a volatile pointer.
*/
#define BOTAN_USE_VOLATILE_MEMSET_FOR_ZERO 1

/*
* Normally blinding is performed by choosing a random starting point (plus
* its inverse, of a form appropriate to the algorithm being blinded), and
* then choosing new blinding operands by successive squaring of both
* values. This is much faster than computing a new starting point but
* introduces some possible corelation
*
* To avoid possible leakage problems in long-running processes, the blinder
* periodically reinitializes the sequence. This value specifies how often
* a new sequence should be started.
*/
#define BOTAN_BLINDING_REINIT_INTERVAL 64

/*
* Userspace RNGs like HMAC_DRBG will reseed after a specified number
* of outputs are generated. Set to zero to disable automatic reseeding.
*/
#define BOTAN_RNG_DEFAULT_RESEED_INTERVAL 1024
#define BOTAN_RNG_RESEED_POLL_BITS 256

#define BOTAN_RNG_AUTO_RESEED_TIMEOUT std::chrono::milliseconds(10)
#define BOTAN_RNG_RESEED_DEFAULT_TIMEOUT std::chrono::milliseconds(50)

/*
* Specifies (in order) the list of entropy sources that will be used
* to seed an in-memory RNG. The first in the default list: "rdseed"
* and "rdrand" do not count as contributing any entropy but are
* included as they are fast and help protect against a seriously
* broken system RNG.
*/
#define BOTAN_ENTROPY_DEFAULT_SOURCES \
   { "rdseed", "rdrand", "p9_darn", "getentropy", "dev_random", \
     "system_rng", "proc_walk", "system_stats" }

/* Multiplier on a block cipher's native parallelism */
#define BOTAN_BLOCK_CIPHER_PAR_MULT 4

/*
* These control the RNG used by the system RNG interface
*/
#define BOTAN_SYSTEM_RNG_DEVICE "/dev/urandom"
#define BOTAN_SYSTEM_RNG_POLL_DEVICES { "/dev/urandom", "/dev/random" }

/*
* This directory will be monitored by ProcWalking_EntropySource and
* the contents provided as entropy inputs to the RNG. May also be
* usefully set to something like "/sys", depending on the system being
* deployed to. Set to an empty string to disable.
*/
#define BOTAN_ENTROPY_PROC_FS_PATH "/proc"

/*
* These paramaters control how many bytes to read from the system
* PRNG, and how long to block if applicable. The timeout only applies
* to reading /dev/urandom and company.
*/
#define BOTAN_SYSTEM_RNG_POLL_REQUEST 64
#define BOTAN_SYSTEM_RNG_POLL_TIMEOUT_MS 20

/*
* When a PBKDF is self-tuning parameters, it will attempt to take about this
* amount of time to self-benchmark.
*/
#define BOTAN_PBKDF_TUNING_TIME std::chrono::milliseconds(10)

/*
* If no way of dynamically determining the cache line size for the
* system exists, this value is used as the default. Used by the side
* channel countermeasures rather than for alignment purposes, so it is
* better to be on the smaller side if the exact value cannot be
* determined. Typically 32 or 64 bytes on modern CPUs.
*/
#if !defined(BOTAN_TARGET_CPU_DEFAULT_CACHE_LINE_SIZE)
  #define BOTAN_TARGET_CPU_DEFAULT_CACHE_LINE_SIZE 32
#endif

/**
* Controls how AutoSeeded_RNG is instantiated
*/
#if !defined(BOTAN_AUTO_RNG_HMAC)

  #if defined(BOTAN_HAS_SHA2_64)
    #define BOTAN_AUTO_RNG_HMAC "HMAC(SHA-384)"
  #elif defined(BOTAN_HAS_SHA2_32)
    #define BOTAN_AUTO_RNG_HMAC "HMAC(SHA-256)"
  #elif defined(BOTAN_HAS_SHA3)
    #define BOTAN_AUTO_RNG_HMAC "HMAC(SHA-3(256))"
  #elif defined(BOTAN_HAS_SHA1)
    #define BOTAN_AUTO_RNG_HMAC "HMAC(SHA-1)"
  #endif
  /* Otherwise, no hash found: leave BOTAN_AUTO_RNG_HMAC undefined */

#endif

/* Check for a common build problem */

#if defined(BOTAN_TARGET_ARCH_IS_X86_64) && ((defined(_MSC_VER) && !defined(_WIN64)) || \
                                             (defined(__clang__) && !defined(__x86_64__)) || \
                                             (defined(__GNUG__) && !defined(__x86_64__)))
    #error "Trying to compile Botan configured as x86_64 with non-x86_64 compiler."
#endif

#if defined(BOTAN_TARGET_ARCH_IS_X86_32) && ((defined(_MSC_VER) && defined(_WIN64)) || \
                                             (defined(__clang__) && !defined(__i386__)) || \
                                             (defined(__GNUG__) && !defined(__i386__)))

    #error "Trying to compile Botan configured as x86_32 with non-x86_32 compiler."
#endif

#include <botan/compiler.h>

#endif

