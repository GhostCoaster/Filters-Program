cmake_minimum_required(VERSION 3.16)
project(Filters_Program C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable( Filters_Program
	src/main.cpp
	src/type.h
	src/img/image.h
	src/img/image.cpp
	src/filter/filter.h
	src/filter/util.h
	src/filter/util.cpp
        src/filter/parameter.h
	src/filter/filter.cpp
	src/commandLine.cpp
	src/commandLine.h
	src/filter/filters/blur.cpp
	src/filter/filters/channelRotate.cpp
	src/filter/filters/merge.cpp
	src/filter/filters/poster.cpp
	src/filter/filters/shred.cpp
	src/filter/filters/shockwave.cpp
	src/filter/filters/filters.h
	src/filter/filters/noise.cpp
	src/filter/filters/NoiseScale.cpp
		src/filter/filters/cut.cpp
	src/filter/filters/sortFilter.cpp
		src/img/buffers.h
	src/img/buffers.cpp
	src/filter/filterList.h
	src/filter/filterList.cpp
	src/help.h
	src/help.cpp
        src/filter/filters/puzzle.cpp src/filter/filters/splatter.cpp src/filter/filters/smoothsplatter.cpp src/filter/filters/glow.cpp)

set_target_properties(Filters_Program PROPERTIES OUTPUT_NAME "fpp")

include(user/CMakeLists.txt)
