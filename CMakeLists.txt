cmake_minimum_required(VERSION 2.8)
project(lru)

add_definitions("-std=c++11")

file(GLOB cpp_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(cpp_file ${cpp_files})
    string(REGEX REPLACE ".cpp\$" "" cpp_name "${cpp_file}")
    add_executable(${cpp_name} ${cpp_file})
endforeach()
