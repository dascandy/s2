#
# Copyright (C) MyExampleCompany. All rights reserved.
#

#set(CMAKE_CXX_COMPILER /usr/bin/clang++)
#set(CMAKE_CC_COMPILER /usr/bin/clang)
set(CMAKE_CXX_FLAGS "-std=c++1z -g -pthread")

#if (COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  set(CMAKE_EXE_LINKER_FLAGS "--coverage")
#endif()


cmake_minimum_required(VERSION 3.7)

add_subdirectory(include)
add_subdirectory(tests)
