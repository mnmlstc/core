add_custom_target(check COMMAND ${CMAKE_TEST_COMMAND})
find_package(unittest REQUIRED)

include_directories(${UNITTEST_INCLUDE_DIR})

#------------------------------------------------------------------------------
# Executable Section
#------------------------------------------------------------------------------
add_executable(test-polymorphic "${TEST_SOURCE_DIR}/polymorphic.cpp")
add_executable(test-iterator "${TEST_SOURCE_DIR}/iterator.cpp")
add_executable(test-expected "${TEST_SOURCE_DIR}/expected.cpp")
add_executable(test-optional "${TEST_SOURCE_DIR}/optional.cpp")
add_executable(test-any "${TEST_SOURCE_DIR}/any.cpp")

#------------------------------------------------------------------------------
# Test Section
#------------------------------------------------------------------------------
add_test(polymorphic test-polymorphic)
add_test(iterator test-iterator)
add_test(expected test-expected)
add_test(optional test-optional)
add_test(any test-any)

#------------------------------------------------------------------------------
# Dependency Section
#------------------------------------------------------------------------------
add_dependencies(check test-polymorphic)
add_dependencies(check test-iterator)
add_dependencies(check test-expected)
add_dependencies(check test-optional)
add_dependencies(check test-any)
