cmake_minimum_required(VERSION 3.16)
project(Filters_Program C CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable( Filters_Program
	src/main.cpp
	src/type.h
	src/img/formatter.h
	src/img/formatter.cpp
	src/img/image.h
	src/img/image.cpp
	src/filter/filters/blur.h
	src/filter/filters/channelRotate.h
	src/filter/filter.h
	src/filter/filters/merge.h
	src/filter/filters/poster.h
	src/filter/util.h
	src/filter/util.cpp
	src/filter/parameter.cpp
	src/filter/parameter.h
	src/filter/filter.cpp
	src/commandLine.cpp
	src/commandLine.h
	src/filter/filters/blur.cpp
	src/filter/filters/channelRotate.cpp
	src/filter/filters/merge.cpp
	src/filter/filters/poster.cpp
	src/filter/filters/shred.cpp
	src/filter/filters/shred.h
	src/filter/filters/shockwave.cpp
	src/filter/filters/shockwave.h
)

include(user/CMakeLists.txt)
