add_subdirectory(lib)

find_package(elfio REQUIRED)

add_library(sym2obj SHARED libsym2obj.cc)
target_link_libraries(sym2obj PRIVATE lsym2obj elfio::elfio)
