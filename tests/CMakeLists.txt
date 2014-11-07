#------------------------------------------------------------------------------
# Modules
#------------------------------------------------------------------------------
include(CheckCXXCompilerFlag)
include(CheckIncludeFileCXX)
include(ExternalProject)

#------------------------------------------------------------------------------
# Macros and Functions
#------------------------------------------------------------------------------
function(require_cxx_include_file header)
  if (CAN_USE_STDLIB_LIBCXX AND BUILD_WITH_LIBCXX)
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${USE_STDLIB_LIBCXX}")
  endif ()
  if (CAN_USE_STD_CXX11)
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${USE_STD_CXX11}")
  endif ()
  check_include_file_cxx(${header} variable)
  if (NOT variable)
    message(FATAL_ERROR "Header <${header}> could not be found")
  endif ()
endfunction ()

function(add_unit_test name file)
  set(LIBCXX $<BOOL:${CAN_USE_STDLIB_LIBCXX}>,$<BOOL:${BUILD_WITH_LIBCXX}>)
  add_executable(test-${name} ${file})
  add_test(${name} test-${name})
  add_dependencies(check test-${name})
  add_dependencies(test-${name} catch) # Always make sure catch is downloaded
  target_compile_options(test-${name}
    PRIVATE
      $<$<AND:${LIBCXX}>:${USE_STDLIB_LIBCXX}>
      $<$<BOOL:${CAN_USE_STD_CXX11}>:${USE_STD_CXX11}>
      $<$<BOOL:${CAN_USE_PEDANTIC}>:${USE_PEDANTIC}>

      $<$<BOOL:${CAN_WARN_EVERYTHING}>:${WARN_EVERYTHING}>
      $<$<BOOL:${CAN_WARN_NOEXCEPT}>:${WARN_NOEXCEPT}>
      $<$<BOOL:${CAN_WARN_EXTRA}>:${WARN_EXTRA}>
      $<$<BOOL:${CAN_WARN_ALL}>:${WARN_ALL}>

      $<$<BOOL:${CAN_WARN_NO_INTERNAL_DECL}>:${WARN_NO_INTERNAL_DECL}>
      $<$<BOOL:${CAN_WARN_NO_CXX98_COMPAT}>:${WARN_NO_CXX98_COMPAT}>
      $<$<BOOL:${CAN_WARN_NO_WEAK_VTABLES}>:${WARN_NO_WEAK_VTABLES}>
      $<$<BOOL:${CAN_WARN_NO_UNUSED_MEMFN}>:${WARN_NO_UNUSED_MEMFN}>
      $<$<BOOL:${CAN_WARN_NO_UNUSED_FUNC}>:${WARN_NO_UNUSED_FUNC}>
      $<$<BOOL:${CAN_WARN_NO_FLOAT_EQUAL}>:${WARN_NO_FLOAT_EQUAL}>
      $<$<BOOL:${CAN_WARN_NO_DEPRECATED}>:${WARN_NO_DEPRECATED}>
      $<$<BOOL:${CAN_WARN_NO_EXIT_TIME}>:${WARN_NO_EXIT_TIME}>
      $<$<BOOL:${CAN_WARN_NO_SHADOW}>:${WARN_NO_SHADOW}>
      $<$<BOOL:${CAN_WARN_NO_PADDED}>:${WARN_NO_PADDED}>)
  target_include_directories(test-${name} SYSTEM PRIVATE ${CATCH_INCLUDE_DIR})
  target_include_directories(test-${name}
    PRIVATE
      $<TARGET_PROPERTY:core,INTERFACE_INCLUDE_DIRECTORIES>)
endfunction ()

#------------------------------------------------------------------------------
# Compiler Environment Check
#------------------------------------------------------------------------------
set(USE_STDLIB_LIBCXX "-stdlib=libc++")
set(USE_STD_CXX11 "-std=c++11")

set(USE_PEDANTIC "-pedantic")

set(WARN_EVERYTHING "-Weverything")
set(WARN_NOEXCEPT "-Wnoexcept")
set(WARN_ONE_LINE "-WL")
set(WARN_ERROR "-Werror")
set(WARN_EXTRA "-Wextra")
set(WARN_ALL "-Wall")

set(WARN_NO_INTERNAL_DECL "-Wno-unneeded-internal-declaration")
set(WARN_NO_CXX98_COMPAT "-Wno-c++98-compat")
set(WARN_NO_WEAK_VTABLES "-Wno-weak-vtables")
set(WARN_NO_UNUSED_MEMFN "-Wno-unused-member-function")
set(WARN_NO_UNUSED_FUNC "-Wno-unused-function")
set(WARN_NO_FLOAT_EQUAL "-Wno-float-equal")
set(WARN_NO_DEPRECATED "-Wno-deprecated-declarations")
set(WARN_NO_EXIT_TIME "-Wno-exit-time-destructors")
set(WARN_NO_SHADOW "-Wno-shadow")
set(WARN_NO_PADDED "-Wno-padded")

# MSVC is not supported, but hopefully it will be one day!
if (MSVC)
  set(WARN_EVERYTHING "/Wall")
  set(WARN_ERROR "/Werror")
  set(WARN_ALL "/W4")
endif ()

check_cxx_compiler_flag(${USE_STDLIB_LIBCXX} CAN_USE_STDLIB_LIBCXX)
check_cxx_compiler_flag(${USE_STD_CXX11} CAN_USE_STD_CXX11)

check_cxx_compiler_flag(${USE_PEDANTIC} CAN_USE_PEDANTIC)

check_cxx_compiler_flag(${WARN_EVERYTHING} CAN_WARN_EVERYTHING)
check_cxx_compiler_flag(${WARN_NOEXCEPT} CAN_WARN_NOEXCEPT)
check_cxx_compiler_flag(${WARN_ONE_LINE} CAN_WARN_ONE_LINE)
check_cxx_compiler_flag(${WARN_ERROR} CAN_WARN_ERROR)
check_cxx_compiler_flag(${WARN_EXTRA} CAN_WARN_EXTRA)
check_cxx_compiler_flag(${WARN_ALL} CAN_WARN_ALL)

check_cxx_compiler_flag(${WARN_NO_INTERNAL_DECL} CAN_WARN_NO_INTERNAL_DECL)
check_cxx_compiler_flag(${WARN_NO_CXX98_COMPAT} CAN_WARN_NO_CXX98_COMPAT)
check_cxx_compiler_flag(${WARN_NO_WEAK_VTABLES} CAN_WARN_NO_WEAK_VTABLES)
check_cxx_compiler_flag(${WARN_NO_UNUSED_MEMFN} CAN_WARN_NO_UNUSED_MEMFN)
check_cxx_compiler_flag(${WARN_NO_UNUSED_FUNC} CAN_WARN_NO_UNUSED_FUNC)
check_cxx_compiler_flag(${WARN_NO_FLOAT_EQUAL} CAN_WARN_NO_FLOAT_EQUAL)
check_cxx_compiler_flag(${WARN_NO_DEPRECATED} CAN_WARN_NO_DEPRECATED)
check_cxx_compiler_flag(${WARN_NO_EXIT_TIME} CAN_WARN_NO_EXIT_TIME)
check_cxx_compiler_flag(${WARN_NO_SHADOW} CAN_WARN_NO_SHADOW)
check_cxx_compiler_flag(${WARN_NO_PADDED} CAN_WARN_NO_PADDED)


require_cxx_include_file(initializer_list)
require_cxx_include_file(type_traits)
require_cxx_include_file(tuple)

require_cxx_include_file(cstdint)
require_cxx_include_file(cstddef)

#------------------------------------------------------------------------------
# Configuration
#------------------------------------------------------------------------------
ExternalProject_Add(catch
  PREFIX ${PROJECT_BINARY_DIR}/catch
  GIT_REPOSITORY git://github.com/philsquared/Catch.git
  GIT_TAG d4e5f184369ce34592bb6f89e793bdb22d1d011a
  TIMEOUT 10
  CONFIGURE_COMMAND ""
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  BUILD_COMMAND "")

ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR "${source_dir}/include" CACHE INTERNAL "Catch Framework")

add_custom_target(check COMMAND ${CMAKE_TEST_COMMAND} DEPENDS catch)

add_unit_test(type-traits "${TEST_SOURCE_DIR}/type-traits.cpp")
add_unit_test(functional "${TEST_SOURCE_DIR}/functional.cpp")
add_unit_test(algorithm "${TEST_SOURCE_DIR}/algorithm.cpp")
add_unit_test(iterator "${TEST_SOURCE_DIR}/iterator.cpp")
add_unit_test(optional "${TEST_SOURCE_DIR}/optional.cpp")
add_unit_test(variant "${TEST_SOURCE_DIR}/variant.cpp")
add_unit_test(utility "${TEST_SOURCE_DIR}/utility.cpp")
add_unit_test(numeric "${TEST_SOURCE_DIR}/numeric.cpp")
add_unit_test(memory "${TEST_SOURCE_DIR}/memory.cpp")
add_unit_test(string "${TEST_SOURCE_DIR}/string.cpp")
add_unit_test(range "${TEST_SOURCE_DIR}/range.cpp")
add_unit_test(any "${TEST_SOURCE_DIR}/any.cpp")
