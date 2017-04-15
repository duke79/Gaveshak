cmake_minimum_required (VERSION 2.6)
project (Gaveshak)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}")

# Executable : OpenCLUse.exe
add_executable(OpenCLUse main.cpp)

# Dependency : OpenCL
include_directories(${CMAKE_SOURCE_DIR}/OpenCL/headers)
TARGET_LINK_LIBRARIES(OpenCLUse ${CMAKE_SOURCE_DIR}/lib/pthreadVC2)