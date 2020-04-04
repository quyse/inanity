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
		--enable-modules=certstor_system \
		--disable-modules=certstor_sql,win32_stats \
		$2
	cd ..
	cp -r build/build/include/* include/
	mkdir -p include_$1/botan
	mv include/botan/build.h include_$1/botan/
	grep -oP 'src/lib/[\w/]+\.cpp' build/Makefile | sort -u | sed -E 's/src\/lib\/(.+)\.cpp/'\''\1'\'',/' | sed -E 's/\//./g' > files_$1.txt
	rm -r build
}

rm -rf include{,_win32,_linux,_darwin}
mkdir include

preparePlatform win32 "--cpu=x86_32 --os=windows --cc=msvc --enable-modules=certstor_system_windows"
preparePlatform linux "--cpu=x86_64 --os=linux --cc=clang --enable-modules=certstor_flatfile"
preparePlatform darwin "--cpu=x86_64 --os=macos --cc=clang --enable-modules=certstor_system_macos"

# get list of lib .cpp files
FILES_LINUX="$(cat files_linux.txt)" FILES_WINDOWS="$(cat files_win32.txt)" envsubst < configure.js.in > configure.js

rm -f files_{win32,linux,darwin}.txt
