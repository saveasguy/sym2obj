add_subdirectory(lib)

add_library(sym2obj SHARED libsym2obj.cc)
target_link_libraries(sym2obj PRIVATE lsym2obj)
