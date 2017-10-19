cmake_minimum_required(VERSION 3.8)
project(jfelip_codebase)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
set(SOURCE_FILES main.cpp include/GenericMessages.hpp include/CScopedTimer.hpp include/StringHelpers.hpp)
add_executable(jfelip_codebase ${SOURCE_FILES})