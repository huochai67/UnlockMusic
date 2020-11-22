cmake_minimum_required (VERSION 3.8)

project (UnlockMusic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif(CMAKE_BUILD_TYPE STREQUAL "Release")

aux_source_directory(./srcs/ SRCS)

add_executable(UnlockMusic ${SRCS})

