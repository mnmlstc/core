cmake_minimum_required(VERSION 2.8.11)
project(core CXX)

#------------------------------------------------------------------------------
# Modules, Variables, and Options
#------------------------------------------------------------------------------
include(CMakePackageConfigHelpers)
include(CheckCXXCompilerFlag)
include(CheckIncludeFileCXX)
include(CTest)

set(INCLUDE_INSTALL_DIR "include" CACHE INTERNAL "Header Files")
set(CMAKE_INSTALL_DIR "share/cmake/core" CACHE INTERNAL "CMake Files")

set(CORE_VERSION_MAJOR 0)
set(CORE_VERSION_MINOR 1)
set(CORE_VERSION_PATCH 0)
set(CORE_VERSION
  ${CORE_VERSION_MAJOR}.${CORE_VERSION_MINOR}.${CORE_VERSION_PATCH}
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/configure")

option(BUILD_WITH_LIBCXX "Use libc++ as stdlib" OFF)

if (APPLE AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(BUILD_WITH_LIBCXX ON CACHE BOOL "Use libc++ as stdlib" ON)
endif ()

#------------------------------------------------------------------------------
# Compiler Environment Check
#------------------------------------------------------------------------------
set(USE_STDLIB_LIBCXX "-stdlib=libc++")
set(USE_STD_CXX11 "-std=c++11")

set(USE_EXCEPTIONS "-fexceptions")
set(USE_RTTI "-frtti")

set(USE_PEDANTIC "-pedantic")

set(WARN_EVERYTHING "-Weverything")
set(WARN_NOEXCEPT "-Wnoexcept")
set(WARN_ONE_LINE "-WL")
set(WARN_ERROR "-Werror")
set(WARN_EXTRA "-Wextra")
set(WARN_ALL "-Wall")

# MSVC is currently unsupported, but this is here for when they get their
# act together.
if (MSVC)
  set(USE_EXCEPTIONS "/Ehsc")
  set(USE_PEDANTIC "-Za")

  set(WARN_EVERYTHING "-Wall") # Yes, I am aware of the irony.
  set(WARN_ERROR "-Werror")
  set(WARN_ALL "-W4")
endif ()

check_cxx_compiler_flag(${USE_STDLIB_LIBCXX} CAN_USE_STDLIB_LIBCXX)
check_cxx_compiler_flag(${USE_STD_CXX11} CAN_USE_STD_CXX11)

check_cxx_compiler_flag(${USE_EXCEPTIONS} CAN_USE_EXCEPTIONS)
check_cxx_compiler_flag(${USE_RTTI} CAN_USE_RTTI)

check_cxx_compiler_flag(${USE_PEDANTIC} CAN_USE_PEDANTIC)

check_cxx_compiler_flag(${WARN_EVERYTHING} CAN_WARN_EVERYTHING)
check_cxx_compiler_flag(${WARN_NOEXCEPT} CAN_WARN_NOEXCEPT)
check_cxx_compiler_flag(${WARN_ONE_LINE} CAN_WARN_ONE_LINE)
check_cxx_compiler_flag(${WARN_ERROR} CAN_WARN_ERROR)
check_cxx_compiler_flag(${WARN_EXTRA} CAN_WARN_EXTRA)
check_cxx_compiler_flag(${WARN_ALL} CAN_WARN_ALL)

if (CAN_USE_STDLIB_LIBCXX AND BUILD_WITH_LIBCXX)
  set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${USE_STDLIB_LIBCXX}")
endif ()

if (CAN_USE_STD_CXX11)
  set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${USE_STD_CXX11}")
endif ()

# These headers are required
check_include_file_cxx(initializer_list HAVE_INITIALIZER_LIST)
check_include_file_cxx(type_traits HAVE_TYPE_TRAITS)
check_include_file_cxx(tuple HAVE_TUPLE)

check_include_file_cxx(cstdint HAVE_CSTDINT)
check_include_file_cxx(cstddef HAVE_CSTDDEF)

# Handle Insane Environment
if (NOT HAVE_INITIALIZER_LIST)
  message(FATAL_ERROR "standard header <initializer_list> could not be found")
endif ()

if (NOT HAVE_TYPE_TRAITS)
  message(FATAL_ERROR "standard header <type_traits> could not be found")
endif ()

if (NOT HAVE_TUPLE)
  message(FATAL_ERROR "standard header <tuple> could not be found")
endif ()

#------------------------------------------------------------------------------
# Configuration
#------------------------------------------------------------------------------
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/core-config-version.cmake
  VERSION ${CORE_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/configure/core-config.cmake.in
  ${PROJECT_BINARY_DIR}/core-config.cmake
  INSTALL_DESTINATION "${CMAKE_INSTALL_DIR}"
  PATH_VARS INCLUDE_INSTALL_DIR CMAKE_INSTALL_DIR
)

include_directories(${PROJECT_SOURCE_DIR}/include)

if (CAN_USE_STDLIB_LIBCXX AND BUILD_WITH_LIBCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_STDLIB_LIBCXX}")
endif ()

if (CAN_USE_STD_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_STD_CXX11}")
endif ()

if (CAN_WARN_ALL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARN_ALL}")
endif ()

if (BUILD_TESTING)
  set(CMAKE_TEST_COMMAND ctest)
  set(TEST_SOURCE_DIR "${PROJECT_SOURCE_DIR}/tests")
  set(TEST_BINARY_DIR "${PROJECT_BINARY_DIR}/tests")
  add_subdirectory("${TEST_SOURCE_DIR}" "${TEST_BINARY_DIR}" EXCLUDE_FROM_ALL)
endif ()
