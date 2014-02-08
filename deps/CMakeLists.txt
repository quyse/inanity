add_subdirectory(lua dep-lua)
add_subdirectory(glew dep-glew)
add_subdirectory(harfbuzz dep-harfbuzz)

# bullet
add_subdirectory(bullet)

# freetype
add_subdirectory(freetype/repo dep-freetype)
add_library(inanity-dep-freetype ALIAS freetype)

add_library(inanity-dep-zlib
	zlib/adler32.c
	zlib/compress.c
	zlib/crc32.c
	zlib/deflate.c
	zlib/infback.c
	zlib/inflate.c
	zlib/inftrees.c
	zlib/inffast.c
	zlib/trees.c
	zlib/uncompr.c
	zlib/zutil.c
)

add_library(inanity-dep-sqlite
	sqlite/sqlite3
)

include_directories(zlib)

add_library(inanity-dep-libpng
	libpng/png.c
	libpng/pngerror.c
	libpng/pngget.c
	libpng/pngmem.c
	libpng/pngpread.c
	libpng/pngread.c
	libpng/pngrio.c
	libpng/pngrtran.c
	libpng/pngrutil.c
	libpng/pngset.c
	libpng/pngtrans.c
	libpng/pngwio.c
	libpng/pngwrite.c
	libpng/pngwtran.c
	libpng/pngwutil.c
)
