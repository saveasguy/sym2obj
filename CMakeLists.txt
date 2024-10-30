cmake_minimum_required(VERSION 3.11)
project(sym2obj C CXX)

option(SYM2OBJ_ENABLE_TESTING "Enable testing." ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

if(SYM2OBJ_ENABLE_TESTING)
    add_subdirectory(test)
endif()
