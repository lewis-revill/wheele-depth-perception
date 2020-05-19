set(RUN_WDP_SOURCES
  wdp-app.cpp
  )

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_DEBUG_RUNTIME OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(Boost_USE_DEBUG_RUNTIME ON)
endif()

find_package(Boost 1.65.0 COMPONENTS program_options REQUIRED)
find_package(JPEG 8 REQUIRED)

add_executable(wdp-app ${RUN_WDP_SOURCES})

target_include_directories(wdp-app PUBLIC
  ${PROJECT_BINARY_DIR}
  ${WDP_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${JPEG_INCLUDE_DIRS}
  )

target_link_libraries(wdp-app PUBLIC
  ${Boost_LIBRARIES}
  ${JPEG_LIBRARIES}
  )
