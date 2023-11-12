cmake_minimum_required (VERSION 3.20)

project (UnlockMusic CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif(CMAKE_BUILD_TYPE STREQUAL "Release")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/srcs")

add_executable(UnlockMusic "${SRC_DIR}/UnlockMusic.cpp")

find_package(OpenSSL REQUIRED)
target_link_libraries(UnlockMusic PRIVATE OpenSSL::SSL OpenSSL::Crypto)

