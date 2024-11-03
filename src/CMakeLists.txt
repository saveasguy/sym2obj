add_subdirectory(lib)

find_package(elfio REQUIRED)

add_library(sym2obj SHARED libsym2obj.cc)
target_link_libraries(sym2obj PRIVATE lsym2obj elfio::elfio)
set_target_properties(sym2obj PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/bin/sym2obj
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/sym2obj.py
    ${CMAKE_BINARY_DIR}/bin/sym2obj)
add_custom_target(sym2obj_py ALL DEPENDS ${CMAKE_BINARY_DIR}/bin/sym2obj)