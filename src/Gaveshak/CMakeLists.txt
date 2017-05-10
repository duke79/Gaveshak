cmake_minimum_required (VERSION 2.6)
project (Gaveshak)                                      # Project - Gaveshak

###############################################
# 1. MODULE_NAME
#    
# Example:
# SET(MODULE_NAME Gaveshak)
#                 ^
###############################################
SET(MODULE_NAME Gaveshak)
###############################################

###############################################
AddEXE(${MODULE_NAME} src/main.cpp)
###############################################

#include_directories(${CMAKE_SOURCE_DIR}/lib/gumbo/include)           # gumbo

DependsOn(Utils)
DependsOn(Fetcher)
DependsOn(Parser)
DependsOn(Boost.ProgramOptions)
DependsOn(Boost.Log)
DependsOn(shlwapi)
DependsOn(Gumbo)