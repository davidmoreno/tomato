cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)
project(libtmtcpp)

# Some tweak parameters
SET(SQLITE true CACHE BOOL "Compile with sqlite support")
########

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

add_subdirectory(src)
add_subdirectory(examples)
