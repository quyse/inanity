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

rm -rf include{,_win32,_linux,_darwin{64,32}}
mkdir include

preparePlatform win32 "--cpu=x86_32 --os=windows --cc=msvc --enable-modules=certstor_system_windows --disable-ssse3 --disable-sse4.1 --disable-sse4.2"
preparePlatform linux "--cpu=x86_64 --os=linux --cc=clang --enable-modules=certstor_flatfile"
preparePlatform darwin64 "--cpu=x86_64 --os=macos --cc=clang --enable-modules=certstor_system_macos --disable-modules=sha2_64_bmi2,sha3_bmi2"
preparePlatform darwin32 "--cpu=x86_32 --os=macos --cc=clang --enable-modules=certstor_system_macos"

cmp files_darwin{64,32}.txt

# get list of lib .cpp files
FILES_LINUX="$(cat files_linux.txt)" FILES_WIN32="$(cat files_win32.txt)" FILES_DARWIN="$(cat files_darwin64.txt)" envsubst < configure.js.in > configure.js

rm -f files_{win32,linux,darwin{64,32}}.txt
