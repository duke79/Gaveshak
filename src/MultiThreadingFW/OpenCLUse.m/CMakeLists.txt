cmake_minimum_required (VERSION 2.6)
project (Gaveshak)

# Executable : OpenCLUse.exe
add_executable(OpenCLUse main.cpp)

# Dependency : OpenCL
include_directories(${CMAKE_SOURCE_DIR}/lib/OpenCL/headers)
TARGET_LINK_LIBRARIES(OpenCLUse ${CMAKE_SOURCE_DIR}/lib/OpenCL/bin/pthreadVC2)
add_custom_command(TARGET OpenCLUse POST_BUILD					# Adds a post-build event to MyTest
		COMMAND ${CMAKE_COMMAND} -E copy_if_different			# which executes "cmake - E copy_if_different..."
        "${CMAKE_SOURCE_DIR}/lib/OpenCL/bin/pthreadVC2.dll"     # <--this is in-file
        ${CMAKE_SOURCE_DIR}/bin)								# <--this is out-file path