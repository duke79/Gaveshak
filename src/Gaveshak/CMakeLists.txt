cmake_minimum_required (VERSION 2.6)
project (Gaveshak)                                      # Project - Gaveshak

# Executable
file(GLOB Gaveshak_SRC
    "*.h"
    "*.cpp"
)
add_executable(Gaveshak main.cpp ${Gaveshak_SRC})       # Gaveshak.exe

# Headers
include_directories(.)                                  # Current directory
include_directories(${CMAKE_SOURCE_DIR}/lib/boost)      # Boost | pre-built boost/boost directory can be downloaded, as building boost is cumbersome
