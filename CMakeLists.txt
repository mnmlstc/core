cmake_minimum_required(VERSION 3.0.0)
project(core VERSION 2.0.0 LANGUAGES CXX)

#------------------------------------------------------------------------------
# Modules, Variables, and Options
#------------------------------------------------------------------------------
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED OR CMAKE_CXX_STANDARD_REQUIRED)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_EXTENSIONS OFF) # would enable -std=gnu++11
endif()

set(INCLUDE_INSTALL_DIR "include" CACHE INTERNAL "Header Files")
set(CMAKE_INSTALL_DIR "share/cmake/core" CACHE INTERNAL "CMake Files")
set(DOCS_INSTALL_DIR "share/doc/mnmlstc/core" CACHE INTERNAL "Documentation")

set(TEST_SOURCE_DIR "${PROJECT_SOURCE_DIR}/tests")
set(TEST_BINARY_DIR "${PROJECT_BINARY_DIR}/tests")

set(DOCS_SOURCE_DIR "${PROJECT_SOURCE_DIR}/docs")
set(DOCS_BINARY_DIR "${PROJECT_BINARY_DIR}/docs")

set(PACK_SOURCE_DIR "${PROJECT_SOURCE_DIR}/package")
set(PACK_BINARY_DIR "${PROJECT_BINARY_DIR}/package")

# Used by cmake-dependent-option
list(APPEND RPM_DEP "UNIX" "NOT APPLE" "BUILD_PACKAGE")
list(APPEND PKG_DEP "APPLE" "BUILD_PACKAGE")
list(APPEND MSI_DEP "WIN32" "BUILD_PACKAGE")

option(BUILD_WITH_LIBCXX "Use libc++ as stdlib (affects unittests)" OFF)
option(BUILD_PACKAGE "Build package with CPack" OFF)
option(BUILD_DOCS "Build documentation with Sphinx Documentation Generator" OFF)

option(DISABLE_EXCEPTIONS "Configures Core to not use exceptions" OFF)
option(DISABLE_RTTI "Configures Core to not use RTTI" OFF)

cmake_dependent_option(BUILD_PACKAGE_MSI "Create an MSI" ON "${MSI_DEP}" OFF)
cmake_dependent_option(BUILD_PACKAGE_RPM "Create an RPM" ON "${RPM_DEP}" OFF)
cmake_dependent_option(BUILD_PACKAGE_PKG "Create a PKG" ON "${PKG_DEP}" OFF)
cmake_dependent_option(BUILD_WITH_LIBCXX "Use libc++" OFF "BUILD_TESTING" OFF)
cmake_dependent_option(BUILD_WITH_COVERAGE "Use GCov" OFF "BUILD_TESTING" OFF)

#------------------------------------------------------------------------------
# Configuration
#------------------------------------------------------------------------------
add_library(core INTERFACE)

write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PACK_SOURCE_DIR}/${PROJECT_NAME}-config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DIR}")

target_include_directories(core INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_definitions(core INTERFACE
  $<$<BOOL:${DISABLE_EXCEPTIONS}>:CORE_NO_EXCEPTIONS>
  $<$<BOOL:${DISABLE_RTTI}>:CORE_NO_RTTI>)

export(TARGETS core NAMESPACE mnmlstc:: FILE core-export.cmake)
export(PACKAGE core)

if (BUILD_TESTING)
  set(CMAKE_TEST_COMMAND ctest)
  add_subdirectory("${TEST_SOURCE_DIR}" "${TEST_BINARY_DIR}")
endif ()

if (BUILD_DOCS)
  add_subdirectory("${DOCS_SOURCE_DIR}" "${DOCS_BINARY_DIR}" EXCLUDE_FROM_ALL)
endif ()

if (BUILD_PACKAGE)
  add_subdirectory("${PACK_SOURCE_DIR}" "${PACK_BINARY_DIR}" EXCLUDE_FROM_ALL)
endif ()

#------------------------------------------------------------------------------
# Install
#------------------------------------------------------------------------------
install(FILES
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  DESTINATION ${CMAKE_INSTALL_DIR})

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/core"
  DESTINATION ${INCLUDE_INSTALL_DIR}
  FILES_MATCHING PATTERN "*.hpp")

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)
install(
  EXPORT ${PROJECT_NAME}-targets
  NAMESPACE mnmlstc::
  DESTINATION ${CMAKE_INSTALL_DIR})

if (BUILD_DOCS)
  install(DIRECTORY "${DOCS_BINARY_DIR}/"
    DESTINATION ${DOCS_INSTALL_DIR}
    FILES_MATCHING
    PATTERN ".buildinfo" EXCLUDE
    PATTERN "CMakeFiles" EXCLUDE
    PATTERN ".doctrees" EXCLUDE
    PATTERN "Makefile" EXCLUDE
    PATTERN "*.cmake" EXCLUDE
    PATTERN "*"
  )
endif ()
