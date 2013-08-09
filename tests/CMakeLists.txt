add_custom_target(check COMMAND ${CMAKE_TEST_COMMAND})
find_package(unittest REQUIRED)

#------------------------------------------------------------------------------
# Executable Section
#------------------------------------------------------------------------------
add_executable(test-iterator "${TEST_SOURCE_DIR}/iterator.cpp")
add_executable(test-any "${TEST_SOURCE_DIR}/any.cpp")

#------------------------------------------------------------------------------
# Test Section
#------------------------------------------------------------------------------
add_test(iterator test-iterator)
add_test(any test-any)

#------------------------------------------------------------------------------
# Dependency Section
#------------------------------------------------------------------------------
add_dependencies(check test-iterator)
add_dependencies(check test-any)
