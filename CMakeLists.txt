cmake_minimum_required(VERSION 3.22)
project(tico-engine)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src/core)
link_libraries(runtime)

add_subdirectory(src/tetris)
add_subdirectory(src/smb)
add_subdirectory(src/snake)
