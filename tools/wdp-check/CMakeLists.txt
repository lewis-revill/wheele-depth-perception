set(WDP_CHECK_SOURCES
  wdp-check.cpp
  )

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_DEBUG_RUNTIME OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(Boost_USE_DEBUG_RUNTIME ON)
endif()

find_package(Boost 1.65.0 COMPONENTS program_options REQUIRED)
find_package(JPEG 8 REQUIRED)

add_executable(wdp-check ${WDP_CHECK_SOURCES})

target_include_directories(wdp-check PUBLIC
  ${PROJECT_BINARY_DIR}
  ${WDP_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${JPEG_INCLUDE_DIRS}
  )

target_link_libraries(wdp-check PUBLIC
  stdc++fs
  ${Boost_LIBRARIES}
  ${JPEG_LIBRARIES}
  )
