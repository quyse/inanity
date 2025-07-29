#ifndef BOTAN_TARGET_INFO_H_
#define BOTAN_TARGET_INFO_H_

#include <botan/build.h>

/**
* @file  target_info.h
*
* Automatically generated from
* 'configure.py --with-build-dir=../build --module-policy=modern --link-method=copy --with-zlib --with-boost --enable-modules=asio,certstor_system --disable-modules=ffi,certstor_sql,win32_stats --disable-rdrand --disable-rdseed --disable-shared-library --disable-cc-tests --cpu=x86_64 --os=windows --cc=clang --enable-modules=certstor_system_windows --disable-ssse3 --disable-sse4.1 --disable-sse4.2'
*
* Target
*  - Compiler: clang++ -fstack-protector -m64 -pthread -std=c++20 -D_REENTRANT -O3
*  - Arch: x86_64
*  - OS: windows
*/

/*
* Configuration
*/
#define BOTAN_CT_VALUE_BARRIER_USE_ASM

[[maybe_unused]] static constexpr bool OptimizeForSize = false;



/*
* Compiler Information
*/
#define BOTAN_BUILD_COMPILER_IS_CLANG

#define BOTAN_COMPILER_INVOCATION_STRING "clang++ -fstack-protector -m64 -pthread -O3"

#define BOTAN_USE_GCC_INLINE_ASM

/*
* External tool settings
*/




/*
* CPU feature information
*/
#define BOTAN_TARGET_ARCH "x86_64"

#define BOTAN_TARGET_ARCH_IS_X86_64

#define BOTAN_TARGET_CPU_IS_X86_FAMILY

#define BOTAN_TARGET_CPU_SUPPORTS_AESNI
#define BOTAN_TARGET_CPU_SUPPORTS_AVX2
#define BOTAN_TARGET_CPU_SUPPORTS_AVX512
#define BOTAN_TARGET_CPU_SUPPORTS_BMI2
#define BOTAN_TARGET_CPU_SUPPORTS_GFNI
#define BOTAN_TARGET_CPU_SUPPORTS_SHA
#define BOTAN_TARGET_CPU_SUPPORTS_SHA512
#define BOTAN_TARGET_CPU_SUPPORTS_SM3
#define BOTAN_TARGET_CPU_SUPPORTS_SM4
#define BOTAN_TARGET_CPU_SUPPORTS_SSE2
#define BOTAN_TARGET_CPU_SUPPORTS_VAES


/*
* Operating system information
*/
#define BOTAN_TARGET_OS_IS_WINDOWS

#define BOTAN_TARGET_OS_HAS_ATOMICS
#define BOTAN_TARGET_OS_HAS_CERTIFICATE_STORE
#define BOTAN_TARGET_OS_HAS_RTLGENRANDOM
#define BOTAN_TARGET_OS_HAS_RTLSECUREZEROMEMORY
#define BOTAN_TARGET_OS_HAS_SYSTEM_CLOCK
#define BOTAN_TARGET_OS_HAS_THREAD_LOCAL
#define BOTAN_TARGET_OS_HAS_VIRTUAL_LOCK
#define BOTAN_TARGET_OS_HAS_WIN32
#define BOTAN_TARGET_OS_HAS_WINSOCK2


/*
* System paths
*/
#define BOTAN_INSTALL_PREFIX R"(c:\Botan)"
#define BOTAN_INSTALL_HEADER_DIR R"(include/botan-3)"
#define BOTAN_INSTALL_LIB_DIR R"(c:\Botan/lib)"
#define BOTAN_LIB_LINK "-lcrypt32 -lws2_32 -lzlib"
#define BOTAN_LINK_FLAGS "-fstack-protector -m64 -pthread"

#define BOTAN_SYSTEM_CERT_BUNDLE "/etc/ssl/certs/ca-certificates.crt"

#endif
