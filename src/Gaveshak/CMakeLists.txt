cmake_minimum_required (VERSION 2.6)
project (Gaveshak)                                      # Project - Gaveshak

# Executable
file(GLOB Gaveshak_SRC
    "*.h"
    "*.cpp"
)
file(GLOB TinyXML2_SRC
    "${CMAKE_SOURCE_DIR}/lib/TinyXML2/*.h"
    "${CMAKE_SOURCE_DIR}/lib/TinyXML2/*.cpp"
)
add_executable(Gaveshak main.cpp ${Gaveshak_SRC} ${TinyXML2_SRC})       # Gaveshak.exe

# Headers
include_directories(.)                                               # Current directory
include_directories(${CMAKE_SOURCE_DIR}/lib/boost)                   # Boost | pre-built boost/boost directory can be downloaded, as building boost is cumbersome
include_directories(${CMAKE_SOURCE_DIR}/lib/libcURL/include)         # libcURL
include_directories(${CMAKE_SOURCE_DIR}/lib/TinyXML2)                # TinyXML2

# Resources
configure_file(values/UserAgents.xml ${CMAKE_SOURCE_DIR}/bin/values/UserAgents.xml @COPYONLY)

# Dependencies
## Boost
SET(BOOST_PROGRAM_OPTIONS_RELEASE ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/libboost_program_options-vc140-mt-1_64)    #Boost::Program_Options : Release
SET(BOOST_PROGRAM_OPTIONS_DEBUG ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/libboost_program_options-vc140-mt-gd-1_64)   #Boost::Program_Options : Debug
SET(BOOST_PROGRAM_OPTIONS optimized ${BOOST_PROGRAM_OPTIONS_RELEASE} debug ${BOOST_PROGRAM_OPTIONS_DEBUG})
TARGET_LINK_LIBRARIES(Gaveshak ${BOOST_PROGRAM_OPTIONS})

## libcURL
SET(CURLLIB ${CMAKE_SOURCE_DIR}/lib/libcURL/lib//libcurl)                                                            #libcurl
TARGET_LINK_LIBRARIES(Gaveshak ${CURLLIB})
add_custom_command(TARGET Gaveshak POST_BUILD					# Adds a post-build event to MyTest                  #copy dll to bin
		COMMAND ${CMAKE_COMMAND} -E copy_if_different			# which executes "cmake - E copy_if_different..."
        "${CURLLIB}.dll"		                                # <--this is in-file
        ${CMAKE_SOURCE_DIR}/bin)								# <--this is out-file path

## shlwapi
TARGET_LINK_LIBRARIES(Gaveshak shlwapi.lib)