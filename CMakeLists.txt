cmake_minimum_required(VERSION 3.9)
project(wasm3_in_c)

set(target ${CMAKE_PROJECT_NAME})
set(current_directory ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/source ${CMAKE_BINARY_DIR}/m3)

add_executable(${target} main.c)
target_link_libraries(${target} PRIVATE m3)

target_compile_options(${target} PUBLIC -g)
target_compile_options(m3 PUBLIC -g)