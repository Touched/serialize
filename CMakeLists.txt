cmake_minimum_required(VERSION 3.4)
project(serialize)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.60.0 COMPONENTS system iostreams REQUIRED)

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.hpp)

add_subdirectory(src)
add_subdirectory(test)
