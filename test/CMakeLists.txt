find_package(GTest REQUIRED)

enable_testing()
add_executable(preload_tests preload_tests.cc)
target_link_libraries(preload_tests PRIVATE gtest::gtest)
