cmake_minimum_required (VERSION 2.6)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# First for the generic no-config case (e.g. with mingw)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

######################################################################################################
#CopyDir()
function(CopyDir DirName)
file(COPY "${DirName}" DESTINATION "${CMAKE_SOURCE_DIR}/bin")
endfunction()

#CopyFile()
function(CopyFile FilePath)
configure_file(${FilePath} ${CMAKE_SOURCE_DIR}/bin/${FilePath} @COPYONLY)
endfunction()

######################################################################################################

function(InitModuleName)
get_filename_component(ThisDirectory ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ThisDirectory ${ThisDirectory})
set(MODULE_NAME ${ThisDirectory} CACHE INTERNAL "")
endfunction()

#BuildEXE()
function(BuildEXE FileWithMain)
InitModuleName()
file(GLOB MODULE_SRC
    "include/*.h"
    "src/*.cpp"
)
add_executable(${MODULE_NAME} ${FileWithMain} ${MODULE_SRC})

include_directories(./include)                                       # Current directory include
include_directories(./src)                                           # Current directory src
endfunction()

#BuildDLL()
function(BuildDLL)
InitModuleName()
set(MODULE_NAME lib${MODULE_NAME} CACHE INTERNAL "")
file(GLOB MODULE_SRC
    "include/*.h"
    "src/*.cpp"
)
include_directories(./include)                                       # Current directory include
include_directories(./src)                                           # Current directory src
add_library(${MODULE_NAME} SHARED ${MODULE_SRC})
endfunction()

######################################################################################################

#DependsOn()
function(DependsOn dependencyName)
## Utils
if(${dependencyName} MATCHES Utils)
include_directories(${CMAKE_SOURCE_DIR}/src/Utils/include)
SET(UTILS ${CMAKE_SOURCE_DIR}/bin/libUtils)
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${UTILS})
endif()

## Fetcher
if(${dependencyName} MATCHES Fetcher)
include_directories(${CMAKE_SOURCE_DIR}/src/Fetcher/include)
SET(FETCHER ${CMAKE_SOURCE_DIR}/bin/libFetcher)
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${FETCHER})
endif()

## Parser
if(${dependencyName} MATCHES Parser)
include_directories(${CMAKE_SOURCE_DIR}/src/Parser/include)
SET(PARSER ${CMAKE_SOURCE_DIR}/bin/libParser)
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${PARSER})
endif()

## Crawler
if(${dependencyName} MATCHES Crawler)
include_directories(${CMAKE_SOURCE_DIR}/src/Crawler/include)
SET(CRAWLER ${CMAKE_SOURCE_DIR}/bin/libCrawler)
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${CRAWLER})
endif()

## Boost.ProgramOptions
if(${dependencyName} MATCHES Boost.ProgramOptions)
add_definitions(-DBOOST_ALL_DYN_LINK) # To force linking with boost dlls intead of libs
include_directories(${CMAKE_SOURCE_DIR}/lib/boost)
SET(BOOST_PROGRAM_OPTIONS_RELEASE ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/boost_program_options-vc140-mt-1_64)    #Boost::Program_Options : Release
SET(BOOST_PROGRAM_OPTIONS_DEBUG ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/boost_program_options-vc140-mt-gd-1_64)   #Boost::Program_Options : Debug
SET(BOOST_PROGRAM_OPTIONS optimized ${BOOST_PROGRAM_OPTIONS_RELEASE} debug ${BOOST_PROGRAM_OPTIONS_DEBUG})
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${BOOST_PROGRAM_OPTIONS})
endif()

## Boost.Log
if(${dependencyName} MATCHES Boost.Log)
add_definitions(-DBOOST_ALL_DYN_LINK) # To force linking with boost dlls intead of libs
include_directories(${CMAKE_SOURCE_DIR}/lib/boost)
SET(BOOST_LOG_RELEASE ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/boost_log-vc140-mt-1_64)                            #Boost::Log : Release
SET(BOOST_LOG_DEBUG ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/boost_log-vc140-mt-gd-1_64)                           #Boost::Log : Debug
SET(BOOST_LOG optimized ${BOOST_LOG_RELEASE} debug ${BOOST_LOG_DEBUG})
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${BOOST_LOG})
endif()

## Boost.Regex
if(${dependencyName} MATCHES Boost.Regex)
add_definitions(-DBOOST_ALL_DYN_LINK) # To force linking with boost dlls intead of libs
include_directories(${CMAKE_SOURCE_DIR}/lib/boost)
SET(BOOST_REGEX_RELEASE ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/boost_regex-vc140-mt-1_64)                        #Boost::Regex : Release
SET(BOOST_REGEX_DEBUG ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/boost_regex-vc140-mt-gd-1_64)                       #Boost::Regex : Debug
SET(BOOST_REGEX optimized ${BOOST_REGEX_RELEASE} debug ${BOOST_REGEX_DEBUG})
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${BOOST_REGEX})
endif()

## shlwapi
if(${dependencyName} MATCHES shlwapi)
TARGET_LINK_LIBRARIES(${MODULE_NAME} shlwapi.lib)
endif()

## Gumbo
if(${dependencyName} MATCHES Gumbo)
include_directories(${CMAKE_SOURCE_DIR}/lib/gumbo/include)
SET(GUMBO_RELEASE ${CMAKE_SOURCE_DIR}/lib/gumbo/lib/gumbo_release)                #Gumbo : Release
SET(GUMBO_DEBUG ${CMAKE_SOURCE_DIR}/lib/gumbo/lib/gumbo_debug)                    #Gumbo : Debug
SET(BOOST_LOG optimized ${GUMBO_RELEASE} debug ${GUMBO_DEBUG})
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${GUMBO_RELEASE})
endif()

## LibCurl
if(${dependencyName} MATCHES LibCurl)
include_directories(${CMAKE_SOURCE_DIR}/lib/libcURL/include)
SET(CURLLIB ${CMAKE_SOURCE_DIR}/lib/libcURL/lib/libcurl)
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${CURLLIB})
add_custom_command(TARGET ${MODULE_NAME} POST_BUILD					# Adds a post-build event to MyTest                  #copy dll to bin
		COMMAND ${CMAKE_COMMAND} -E copy_if_different			    # which executes "cmake - E copy_if_different..."
        "${CURLLIB}.dll"		                                    # <--this is in-file
        ${CMAKE_SOURCE_DIR}/bin)								    # <--this is out-file path
endif()

## OpenCL
if(${dependencyName} MATCHES OpenCL)
include_directories(${CMAKE_SOURCE_DIR}/lib/OpenCL/headers)
SET(OPENCL ${CMAKE_SOURCE_DIR}/lib/OpenCL/bin/pthreadVC2)
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${OPENCL})
add_custom_command(TARGET ${MODULE_NAME} POST_BUILD					# Adds a post-build event to MyTest
		COMMAND ${CMAKE_COMMAND} -E copy_if_different			# which executes "cmake - E copy_if_different..."
        "${OPENCL}.dll"     # <--this is in-file
        ${CMAKE_SOURCE_DIR}/bin)								# <--this is out-file path
endif()

endfunction()

######################################################################################################

# Add all the Frameworks
add_subdirectory (src)