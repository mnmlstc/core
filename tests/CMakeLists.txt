#------------------------------------------------------------------------------
# Modules
#------------------------------------------------------------------------------
include(CheckCXXCompilerFlag)
include(CheckIncludeFileCXX)

#------------------------------------------------------------------------------
# Macros and Functions
#------------------------------------------------------------------------------
function(require_cxx_include_file header)
  if (CAN_USE_STD_CXX11)
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${USE_STD_CXX11}")
  endif ()
  if (CAN_USE_STDLIB_LIBCXX AND BUILD_WITH_LIBCXX)
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${USE_STDLIB_LIBCXX}")
  endif ()
  string(TOUPPER "${header}" item)
  check_include_file_cxx(${header} ${item})
  if (NOT ${item})
    message(FATAL_ERROR "Header <${header}> could not be found")
  endif ()
endfunction ()

function(add_unit_test name file)
  set(LIBCXX $<BOOL:${CAN_USE_STDLIB_LIBCXX}>,$<BOOL:${BUILD_WITH_LIBCXX}>)
  set(NO_RTTI $<BOOL:${CAN_USE_NO_RTTI}>,$<BOOL:${DISABLE_RTTI}>)
  if (NOT TARGET catch-main)
    add_library(catch-main OBJECT "${TEST_SOURCE_DIR}/catch.cpp")
    target_compile_options(catch-main
      PUBLIC
        $<$<BOOL:${BUILD_WITH_COVERAGE}>:${USE_COVERAGE}>
        $<$<BOOL:${CAN_USE_STD_CXX11}>:${USE_STD_CXX11}>
        $<$<AND:${LIBCXX}>:${USE_STDLIB_LIBCXX}>
        $<$<AND:${NO_RTTI}>:${USE_NO_RTTI}>

        #$<$<BOOL:${CAN_SANITIZE_UNDEFINED}>:${SANITIZE_UNDEFINED}>
        $<$<BOOL:${CAN_SANITIZE_ADDRESS}>:${SANITIZE_ADDRESS}>
        $<$<BOOL:${CAN_SANITIZE_MEMORY}>:${SANITIZE_MEMORY}>

        $<$<BOOL:${CAN_WARN_MAYBE_UNINIT}>:${WARN_MAYBE_UNINIT}>
        $<$<BOOL:${CAN_WARN_EVERYTHING}>:${WARN_EVERYTHING}>
        $<$<BOOL:${CAN_WARN_NOEXCEPT}>:${WARN_NOEXCEPT}>
        $<$<BOOL:${CAN_WARN_PEDANTIC}>:${WARN_PEDANTIC}>
        $<$<BOOL:${CAN_WARN_EXTRA}>:${WARN_EXTRA}>
        $<$<BOOL:${CAN_WARN_ODR}>:${WARN_ODR}>
        $<$<BOOL:${CAN_WARN_ALL}>:${WARN_ALL}>

        $<$<BOOL:${CAN_WARN_NO_CXX98_PEDANTIC}>:${WARN_NO_CXX98_PEDANTIC}>
        $<$<BOOL:${CAN_WARN_NO_POTENTIAL_EXPR}>:${WARN_NO_POTENTIAL_EXPR}>
        $<$<BOOL:${CAN_WARN_NO_INTERNAL_DECL}>:${WARN_NO_INTERNAL_DECL}>
        $<$<BOOL:${CAN_WARN_NO_DOUBLE_PROMO}>:${WARN_NO_DOUBLE_PROMO}>
        $<$<BOOL:${CAN_WARN_NO_CXX98_COMPAT}>:${WARN_NO_CXX98_COMPAT}>
        $<$<BOOL:${CAN_WARN_NO_WEAK_VTABLES}>:${WARN_NO_WEAK_VTABLES}>
        $<$<BOOL:${CAN_WARN_NO_UNUSED_MEMFN}>:${WARN_NO_UNUSED_MEMFN}>
        $<$<BOOL:${CAN_WARN_NO_UNUSED_FUNC}>:${WARN_NO_UNUSED_FUNC}>
        $<$<BOOL:${CAN_WARN_NO_FLOAT_EQUAL}>:${WARN_NO_FLOAT_EQUAL}>
        $<$<BOOL:${CAN_WARN_NO_DEPRECATED}>:${WARN_NO_DEPRECATED}>
        $<$<BOOL:${CAN_WARN_NO_ATTRIBUTES}>:${WARN_NO_ATTRIBUTES}>
        $<$<BOOL:${CAN_WARN_NO_EXIT_TIME}>:${WARN_NO_EXIT_TIME}>
        $<$<BOOL:${CAN_WARN_NO_MACRO_EXP}>:${WARN_NO_MACRO_EXP}>
        $<$<BOOL:${CAN_WARN_NO_SHADOW}>:${WARN_NO_SHADOW}>
        $<$<BOOL:${CAN_WARN_NO_PADDED}>:${WARN_NO_PADDED}>)
  endif ()
  add_executable(test-${name} ${file} $<TARGET_OBJECTS:catch-main>)
  add_test(${name} test-${name})
  add_dependencies(check test-${name})
  target_compile_options(test-${name}
    PRIVATE
      $<TARGET_PROPERTY:catch-main,INTERFACE_COMPILE_OPTIONS>)
  target_include_directories(test-${name} SYSTEM PRIVATE ${TEST_SOURCE_DIR})
  target_link_libraries(test-${name} PUBLIC core)
  target_link_libraries(test-${name} PRIVATE
    $<$<AND:${LIBCXX}>:${USE_STDLIB_LIBCXX}>
    #$<$<BOOL:${CAN_SANITIZE_UNDEFINED}>:${SANITIZE_UNDEFINED}>
    $<$<BOOL:${CAN_SANITIZE_ADDRESS}>:${SANITIZE_ADDRESS}>
    $<$<BOOL:${CAN_SANITIZE_MEMORY}>:${SANITIZE_MEMORY}>
    $<$<BOOL:${BUILD_WITH_COVERAGE}>:${USE_COVERAGE}>)
endfunction ()

#------------------------------------------------------------------------------
# Compiler Environment Check
#------------------------------------------------------------------------------

set(USE_STD_CXX11 "-std=c++11")

set(USE_STDLIB_LIBCXX "-stdlib=libc++")
set(USE_COVERAGE "--coverage")

set(USE_NO_EXCEPTIONS "-fno-exceptions")
set(USE_NO_RTTI "-fno-rtti")

set(SANITIZE_UNDEFINED "-fsanitize=undefined")
set(SANITIZE_ADDRESS "-fsanitize=address")
set(SANITIZE_MEMORY "-fsanitize=memory")

set(WARN_MAYBE_UNINIT "-Wmaybe-unintialized")
set(WARN_EVERYTHING "-Weverything")
set(WARN_NOEXCEPT "-Wnoexcept")
set(WARN_PEDANTIC "-Wpedantic")
set(WARN_ONE_LINE "-WL")
set(WARN_ERROR "-Werror")
set(WARN_EXTRA "-Wextra")
set(WARN_ODR "-Wodr")
set(WARN_ALL "-Wall")

set(WARN_NO_POTENTIAL_EXPR "-Wno-potentially-evaluated-expression")
set(WARN_NO_INTERNAL_DECL "-Wno-unneeded-internal-declaration")
set(WARN_NO_CXX98_PEDANTIC "-Wno-c++98-compat-pedantic")
set(WARN_NO_DOUBLE_PROMO "-Wno-double-promotion")
set(WARN_NO_CXX98_COMPAT "-Wno-c++98-compat")
set(WARN_NO_WEAK_VTABLES "-Wno-weak-vtables")
set(WARN_NO_UNUSED_MEMFN "-Wno-unused-member-function")
set(WARN_NO_UNUSED_FUNC "-Wno-unused-function")
set(WARN_NO_FLOAT_EQUAL "-Wno-float-equal")
set(WARN_NO_DEPRECATED "-Wno-deprecated-declarations")
set(WARN_NO_ATTRIBUTES "-Wno-attributes")
set(WARN_NO_EXIT_TIME "-Wno-exit-time-destructors")
set(WARN_NO_MACRO_EXP "-Wno-disabled-macro-expansion")
set(WARN_NO_SHADOW "-Wno-shadow")
set(WARN_NO_PADDED "-Wno-padded")

# MSVC is not supported, but hopefully it will be one day!
if (MSVC)
  set(WARN_ERROR "/Werror")
  set(WARN_ALL "/W4")
  set(WARN_NO_SHADOW "/wd4458")
endif ()

# Only for when using certain cmake versions (like the ones found on travis-ci)
if (${CMAKE_VERSION} VERSION_LESS "3.1.0" OR "$ENV{TRAVIS}" STREQUAL "true")
  check_cxx_compiler_flag(${USE_STD_CXX11} CAN_USE_STD_CXX11)
endif ()

check_cxx_compiler_flag(${USE_STDLIB_LIBCXX} CAN_USE_STDLIB_LIBCXX)
check_cxx_compiler_flag(${USE_NO_RTTI} CAN_USE_NO_RTTI)

check_cxx_compiler_flag(${SANITIZE_UNDEFINED} CAN_SANITIZE_UNDEFINED)
check_cxx_compiler_flag(${SANITIZE_ADDRESS} CAN_SANITIZE_ADDRESS)
check_cxx_compiler_flag(${SANITIZE_MEMORY} CAN_SANITIZE_MEMORY)

check_cxx_compiler_flag(${WARN_MAYBE_UNINIT} CAN_WARN_MAYBE_UNINIT)
check_cxx_compiler_flag(${WARN_EVERYTHING} CAN_WARN_EVERYTHING)
check_cxx_compiler_flag(${WARN_NOEXCEPT} CAN_WARN_NOEXCEPT)
check_cxx_compiler_flag(${WARN_PEDANTIC} CAN_WARN_PEDANTIC)
check_cxx_compiler_flag(${WARN_ONE_LINE} CAN_WARN_ONE_LINE)
check_cxx_compiler_flag(${WARN_ERROR} CAN_WARN_ERROR)
check_cxx_compiler_flag(${WARN_EXTRA} CAN_WARN_EXTRA)
check_cxx_compiler_flag(${WARN_ODR} CAN_WARN_ODR)
check_cxx_compiler_flag(${WARN_ALL} CAN_WARN_ALL)

check_cxx_compiler_flag(${WARN_NO_POTENTIAL_EXPR} CAN_WARN_NO_POTENTIAL_EXPR)
check_cxx_compiler_flag(${WARN_NO_CXX98_PEDANTIC} CAN_WARN_NO_CXX98_PEDANTIC)
check_cxx_compiler_flag(${WARN_NO_INTERNAL_DECL} CAN_WARN_NO_INTERNAL_DECL)
check_cxx_compiler_flag(${WARN_NO_DOUBLE_PROMO} CAN_WARN_NO_DOUBLE_PROMO)
check_cxx_compiler_flag(${WARN_NO_CXX98_COMPAT} CAN_WARN_NO_CXX98_COMPAT)
check_cxx_compiler_flag(${WARN_NO_WEAK_VTABLES} CAN_WARN_NO_WEAK_VTABLES)
check_cxx_compiler_flag(${WARN_NO_UNUSED_MEMFN} CAN_WARN_NO_UNUSED_MEMFN)
check_cxx_compiler_flag(${WARN_NO_UNUSED_FUNC} CAN_WARN_NO_UNUSED_FUNC)
check_cxx_compiler_flag(${WARN_NO_FLOAT_EQUAL} CAN_WARN_NO_FLOAT_EQUAL)
check_cxx_compiler_flag(${WARN_NO_DEPRECATED} CAN_WARN_NO_DEPRECATED)
check_cxx_compiler_flag(${WARN_NO_ATTRIBUTES} CAN_WARN_NO_ATTRIBUTES)
check_cxx_compiler_flag(${WARN_NO_EXIT_TIME} CAN_WARN_NO_EXIT_TIME)
check_cxx_compiler_flag(${WARN_NO_MACRO_EXP} CAN_WARN_NO_MACRO_EXP)
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
add_custom_target(check COMMAND ${CMAKE_TEST_COMMAND})

add_unit_test(propagate-const "${TEST_SOURCE_DIR}/propagate-const.cpp")
add_unit_test(memory-resource "${TEST_SOURCE_DIR}/memory-resource.cpp")
add_unit_test(type-traits "${TEST_SOURCE_DIR}/type-traits.cpp")
add_unit_test(string-view "${TEST_SOURCE_DIR}/string-view.cpp")
add_unit_test(functional "${TEST_SOURCE_DIR}/functional.cpp")
add_unit_test(algorithm "${TEST_SOURCE_DIR}/algorithm.cpp")
add_unit_test(iterator "${TEST_SOURCE_DIR}/iterator.cpp")
add_unit_test(optional "${TEST_SOURCE_DIR}/optional.cpp")
add_unit_test(variant "${TEST_SOURCE_DIR}/variant.cpp")
add_unit_test(utility "${TEST_SOURCE_DIR}/utility.cpp")
add_unit_test(numeric "${TEST_SOURCE_DIR}/numeric.cpp")
add_unit_test(memory "${TEST_SOURCE_DIR}/memory.cpp")
add_unit_test(range "${TEST_SOURCE_DIR}/range.cpp")
add_unit_test(array "${TEST_SOURCE_DIR}/array.cpp")
add_unit_test(any "${TEST_SOURCE_DIR}/any.cpp")
