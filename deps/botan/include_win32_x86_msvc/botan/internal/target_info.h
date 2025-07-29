#ifndef BOTAN_TARGET_INFO_H_
#define BOTAN_TARGET_INFO_H_

#include <botan/build.h>

/**
* @file  target_info.h
*
* Automatically generated from
* 'configure.py --with-build-dir=../build --module-policy=modern --link-method=copy --with-zlib --with-boost --enable-modules=asio,certstor_system --disable-modules=ffi,certstor_sql,win32_stats --disable-rdrand --disable-rdseed --disable-shared-library --disable-cc-tests --cpu=x86_32 --os=windows --cc=msvc --enable-modules=certstor_system_windows --disable-ssse3 --disable-sse4.1 --disable-sse4.2'
*
* Target
*  - Compiler: cl  /Zc:preprocessor /std:c++20 /EHs /GR /MD /bigobj /O2 /Oi /Zc:throwingNew
*  - Arch: x86_32
*  - OS: windows
*/

/*
* Configuration
*/

[[maybe_unused]] static constexpr bool OptimizeForSize = false;



/*
* Compiler Information
*/
#define BOTAN_BUILD_COMPILER_IS_MSVC

#define BOTAN_COMPILER_INVOCATION_STRING "cl  /MD /bigobj /O2 /Oi /Zc:throwingNew"


/*
* External tool settings
*/




/*
* CPU feature information
*/
#define BOTAN_TARGET_ARCH "x86_32"

#define BOTAN_TARGET_ARCH_IS_X86_32

#define BOTAN_TARGET_CPU_IS_X86_FAMILY

#define BOTAN_TARGET_CPU_SUPPORTS_AESNI
#define BOTAN_TARGET_CPU_SUPPORTS_SHA
#define BOTAN_TARGET_CPU_SUPPORTS_SSE2


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
#define BOTAN_LIB_LINK "crypt32.lib ws2_32.lib zlib.lib"
#define BOTAN_LINK_FLAGS ""

#define BOTAN_SYSTEM_CERT_BUNDLE "/etc/ssl/certs/ca-certificates.crt"

#endif
