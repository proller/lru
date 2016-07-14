#
# cmake .
# cmake . -DCMAKE_CXX_COMPILER=`which \`bash -c "compgen -c clang | grep 'clang++-[[:digit:]]' | sort --version-sort --reverse | head -n1"\``
#
cmake_minimum_required(VERSION 2.8)
project(lru)

add_definitions("-std=c++11")

option(ASAN "" 0)
option(USAN "" 0)
option(TSAN "" 0)
option(MSAN "" 0)
if(ASAN)
  set(SAN_FLAGS "-fsanitize=address -fPIE -fno-omit-frame-pointer -fno-optimize-sibling-calls")
elseif(TSAN)
  set(SAN_FLAGS "-fsanitize=thread -fPIE -fno-omit-frame-pointer -fno-optimize-sibling-calls")
elseif(MSAN)
  set(SAN_FLAGS "-fsanitize=memory -fPIE -fno-omit-frame-pointer -fno-optimize-sibling-calls")
elseif(USAN)
  set(SAN_FLAGS "-fsanitize=undefined -fPIE -fno-omit-frame-pointer -fno-optimize-sibling-calls")
endif()

if (SAN_FLAGS)
  add_definitions(${SAN_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SAN_FLAGS}")
endif()

option(DEBUG "With debug info and witout optimizations" 0)
if(SAN_FLAGS)
  set(DEBUG 1)
endif()
if(DEBUG)
  add_definitions("-g -O0 ")
else()
  add_definitions("-DNDEBUG")
endif()


file(GLOB cpp_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(cpp_file ${cpp_files})
  string(REGEX REPLACE ".cpp\$" "" cpp_name "${cpp_file}")
  add_executable(${cpp_name} ${cpp_file})
endforeach()
