cmake_minimum_required(VERSION 3.16)

project(uwhook LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(UWHOOK_BUILD_EXAMPLE "Build UWHook example" OFF)

add_library(uwhook STATIC
  uwhook.cpp
  include/uwhook.hpp
  include/uwhook_c.h
  uwhook_c.cpp
)
target_include_directories(uwhook PUBLIC include)

if (UWHOOK_BUILD_EXAMPLE)
    add_executable(uwhook_example example.cpp)
    target_link_libraries(uwhook_example PRIVATE uwhook)
endif()
