#!/bin/bash

set -e

function preparePlatform {
	rm -rf build
	mkdir build
	cd repo
	./configure.py \
		--with-build-dir=../build \
		--module-policy=modern \
		--link-method=copy \
		--with-zlib \
		--with-boost \
		--enable-modules=asio,certstor_system \
		--disable-modules=ffi,certstor_sql,win32_stats \
		--disable-rdrand --disable-rdseed \
		--disable-shared-library \
		$2
	cd ..
	cp -r build/build/include/* include/
	mkdir -p include_$1/botan
	mv include/botan/build.h include_$1/botan/
	grep -oP 'src/lib/[\w/]+\.cpp' build/Makefile | sort -u | sed -E 's/src\/lib\/(.+)\.cpp/'\''\1'\'',/' | sed -E 's/\//./g' > files_$1.txt
	rm -r build
}

rm -rf include{,_{win32,linux,darwin}_x{64,86}}
mkdir include

preparePlatform win32_x64_clang "--cpu=x86_64 --os=windows --cc=clang --enable-modules=certstor_system_windows --disable-ssse3 --disable-sse4.1 --disable-sse4.2"
preparePlatform win32_x86_msvc "--cpu=x86_32 --os=windows --cc=msvc --enable-modules=certstor_system_windows --disable-ssse3 --disable-sse4.1 --disable-sse4.2"
preparePlatform linux_x64_clang "--cpu=x86_64 --os=linux --cc=clang --enable-modules=certstor_flatfile"
preparePlatform darwin_x64_clang "--cpu=x86_64 --os=macos --cc=clang --enable-modules=certstor_system_macos --without-os-features=clock_gettime"
preparePlatform darwin_x86_clang "--cpu=x86_32 --os=macos --cc=clang --enable-modules=certstor_system_macos --without-os-features=clock_gettime"

# get list of lib .cpp files
\
	FILES_LINUX64_CLANG="$(cat files_linux_x64_clang.txt)" \
	FILES_WIN64_CLANG="$(cat files_win32_x64_clang.txt)" \
	FILES_WIN32_MSVC="$(cat files_win32_x86_msvc.txt)" \
	FILES_DARWIN64_CLANG="$(cat files_darwin_x64_clang.txt)" \
	FILES_DARWIN32_CLANG="$(cat files_darwin_x86_clang.txt)" \
	envsubst < configure.js.in > configure.js

rm -f files_{win32,linux,darwin}_x{64,86}_{clang,msvc}.txt
