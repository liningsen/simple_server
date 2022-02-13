cmake_minimum_required(VERSION 3.5.0)

project(testaaa)

set(CMAKE_CXX_FLAGS_RELEASE     "-O2")
set(CMAKE_CXX_FLAGS_DEBUG       "-O0 -g3")
add_subdirectory(src)
