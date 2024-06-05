cmake_minimum_required(VERSION 3.27)
project(pigraid-friend-service-tests)

enable_testing()

# Set the binary directory for GTest
set(GTEST_BINARY_DIR ${CMAKE_BINARY_DIR}/gtest)

# Include the GTest library from the external directory
add_subdirectory(${CMAKE_SOURCE_DIR}/../external/googletest ${GTEST_BINARY_DIR})

# Add the test executable
add_executable(request_dao_test ${CMAKE_SOURCE_DIR}/request_dao_test.cpp)

# Link with Google Test
target_link_libraries(request_dao_test PRIVATE gtest_main)

# Discover and run tests with Google Test
include(GoogleTest)
gtest_discover_tests(request_dao_test)
