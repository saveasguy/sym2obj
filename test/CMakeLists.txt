find_package(GTest REQUIRED)

enable_testing()
add_executable(test preload_tests.cc)
target_link_libraries(test PRIVATE gtest::gtest)

include(GoogleTest)
gtest_discover_tests(test)
