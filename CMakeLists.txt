cmake_minimum_required(VERSION 3.13)

set(SPIRAL_VERSION 1.4)

project(spiral VERSION ${SPIRAL_VERSION} LANGUAGES CXX)


include(cmake/StandardProjectSettings.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)


add_subdirectory(src)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET spiral PROPERTY WIN32_EXECUTABLE true)
endif()
