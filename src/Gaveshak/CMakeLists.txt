cmake_minimum_required (VERSION 2.6)
project (Gaveshak)

# Executable : OpenCLUse.exe
file(GLOB Gaveshak_SRC
    "*.h"
    "*.cpp"
)
add_executable(Gaveshak main.cpp ${Gaveshak_SRC})

# Headers
include_directories(.)
