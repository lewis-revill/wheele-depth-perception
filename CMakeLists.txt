cmake_minimum_required(VERSION 3.10.0)

# Setting up project variables.
if(NOT DEFINED WDP_VERSION_MAJOR)
  set(WDP_VERSION_MAJOR 0)
endif()
if(NOT DEFINED WDP_VERSION_MINOR)
  set(WDP_VERSION_MINOR 0)
endif()
if(NOT DEFINED WDP_VERSION_PATCH)
  set(WDP_VERSION_PATCH 0)
endif()

project(WDP
  VERSION ${WDP_VERSION_MAJOR}.${WDP_VERSION_MINOR}.${WDP_VERSION_PATCH}
  LANGUAGES CXX)

# CMake build variables.

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to conform to")
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (default Debug)" FORCE)
endif()

# Directories and sources.

set(WDP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(WDP_DATA_DIR ${CMAKE_SOURCE_DIR}/data)
set(WDP_TEST_DIR ${CMAKE_SOURCE_DIR}/test)

# add_subdirectory(lib)
add_subdirectory(tools)

configure_file(WDPConfig.h.in WDPConfig.h)
