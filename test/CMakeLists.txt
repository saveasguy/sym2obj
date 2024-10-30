find_package(GTest REQUIRED)

enable_testing()
add_executable(test
    arg_list_test.cc
    preload_test.cc 
    process_test.cc
)
target_link_libraries(test PRIVATE gtest::gtest lsym2obj)

include(GoogleTest)
gtest_discover_tests(test)
