add_custom_target(check COMMAND ${CMAKE_TEST_COMMAND})
find_package(unittest REQUIRED NO_CMAKE_BUILDS_PATH)

include_directories(${UNITTEST_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

#------------------------------------------------------------------------------
# Executable Section
#------------------------------------------------------------------------------
add_executable(test-type-traits "${TEST_SOURCE_DIR}/type-traits.cpp")
add_executable(test-functional "${TEST_SOURCE_DIR}/functional.cpp")
add_executable(test-algorithm "${TEST_SOURCE_DIR}/algorithm.cpp")
add_executable(test-iterator "${TEST_SOURCE_DIR}/iterator.cpp")
add_executable(test-optional "${TEST_SOURCE_DIR}/optional.cpp")
add_executable(test-variant "${TEST_SOURCE_DIR}/variant.cpp")
add_executable(test-utility "${TEST_SOURCE_DIR}/utility.cpp")
add_executable(test-numeric "${TEST_SOURCE_DIR}/numeric.cpp")
add_executable(test-string "${TEST_SOURCE_DIR}/string.cpp")
add_executable(test-memory "${TEST_SOURCE_DIR}/memory.cpp")
add_executable(test-range "${TEST_SOURCE_DIR}/range.cpp")
add_executable(test-any "${TEST_SOURCE_DIR}/any.cpp")

#------------------------------------------------------------------------------
# Test Section
#------------------------------------------------------------------------------
add_test(type-traits test-type-traits)
add_test(functional test-functional)
add_test(algorithm test-algorithm)
add_test(iterator test-iterator)
add_test(optional test-optional)
add_test(variant test-variant)
add_test(utility test-utility)
add_test(numeric test-numeric)
add_test(string test-string)
add_test(memory test-memory)
add_test(range test-range)
add_test(any test-any)

#------------------------------------------------------------------------------
# Dependency Section
#------------------------------------------------------------------------------
add_dependencies(check test-type-traits)
add_dependencies(check test-functional)
add_dependencies(check test-algorithm)
add_dependencies(check test-iterator)
add_dependencies(check test-optional)
add_dependencies(check test-variant)
add_dependencies(check test-utility)
add_dependencies(check test-numeric)
add_dependencies(check test-string)
add_dependencies(check test-memory)
add_dependencies(check test-range)
add_dependencies(check test-any)
