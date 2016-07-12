#
# cmake .
# cmake . -DCMAKE_CXX_COMPILER=`which \`bash -c "compgen -c clang | grep 'clang++-[[:digit:]]' | sort --version-sort --reverse | head -n1"\``
#
cmake_minimum_required(VERSION 2.8)
project(lru)

add_definitions("-std=c++11")

#add_definitions("-g -O0")
#add_definitions("-fsanitize=address -fPIE -fno-omit-frame-pointer -fno-optimize-sibling-calls")
#add_definitions("-fsanitize=memory -fPIE -fno-omit-frame-pointer -fno-optimize-sibling-calls")

file(GLOB cpp_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(cpp_file ${cpp_files})
    string(REGEX REPLACE ".cpp\$" "" cpp_name "${cpp_file}")
    add_executable(${cpp_name} ${cpp_file})
endforeach()
