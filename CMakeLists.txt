cmake_minimum_required(VERSION 3.5.0)
project(yawiel C CXX)

# This CMake file is inspired by mlpack's.

# Compilation options.
option(BUILD_TESTS "Build tests." ON)
option(BUILD_DOCS "Build Doxygen documentation." OFF)
option(STATIC_BUILD "Build using static linking (OMP cannot link)." OFF)
option(OMP "Paralellize using OpenMP." OFF)

# Set C++11 standard.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Boost library.
# This list needs to be updated when new versions of Boost come out.
set(Boost_ADDITIONAL_VERSIONS
  "1.69.0" "1.69" "1.70"
  "1.68.0" "1.68" "1.67.0"
  "1.67" "1.66.0" "1.66"
  "1.65.1" "1.65.0" "1.65"
  "1.64.1" "1.64.0" "1.64"
  "1.63.1" "1.63.0" "1.63"
  "1.62.1" "1.62.0" "1.62"
  "1.61.1" "1.61.0" "1.61"
  "1.60.1" "1.60.0" "1.60"
  "1.59.1" "1.59.0" "1.59"
  "1.58.1" "1.58.0" "1.58")
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost 1.58
    COMPONENTS
      unit_test_framework
      serialization
    REQUIRED )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

# OpenMP flags
if (OMP)
  find_package(OpenMP)
endif()

if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set (CMAKE_EXE_LINKER_FLAGS
       "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src/)

# Build the project sources.
add_subdirectory(src/yawiel)

# Build documentation.
if (BUILD_DOCS)
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    # Generate documentation.
    add_custom_target(doc
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/Doxyfile"
        DEPENDS "${CMAKE_SOURCE_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Build Doxygen documentation"
    )
  endif()
endif()
