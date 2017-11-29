#
# Copyright (C) YourCompany. All rights reserved.
#

project(tests)

add_executable(${PROJECT_NAME}
  functional/test_simple.cpp
  future/test_simple.cpp
  main.cpp
  string/test_cp437.cpp
  string/test_iso8859_15.cpp
  string/test_iso8859_1.cpp
  string/test_simple.cpp
  string/test_ucs2.cpp
  string/test_utf16.cpp
  string/test_utf32.cpp
  string/test_utf8.cpp
  string/test_utf8_modified.cpp
  string/test_windows1252.cpp
  string/test_wtf8.cpp
  vector/test_simple.cpp
)
target_include_directories(${PROJECT_NAME}
  PRIVATE
    ../include
)
