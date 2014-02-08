add_definitions(-DGLEW_STATIC)
include_directories(include)

add_library(inanity-dep-glew
	src/glew.c
)
