cmake_minimum_required (VERSION 2.6)
project (Gaveshak)                                      # Project - Gaveshak

###############################################
# 1. MODULE_NAME
#    
# Example:
# SET(MODULE_NAME Gaveshak)
#                 ^
###############################################
SET(MODULE_NAME Parser)
###############################################


###############################################
# 2. Executable / Library(SHARED/STATIC)
#    
#Example:
# file(GLOB MODULE_SRC
#     "include/*.h"
#     "src/*.cpp"
# )
# add_library(${MODULE_NAME} SHARED ${MODULE_SRC})                               # Fetcher.dll
# ^                          ^                                                    ^
###############################################
file(GLOB MODULE_SRC
    "include/*.h"
    "src/*.cpp"
)
add_library(${MODULE_NAME} SHARED ${MODULE_SRC})                    # Parser.dll
###############################################


###############################################
# 3. Includes
#            Exteral includes & current module's files
#Example:
# include_directories(./include)                                       # Current directory include
# include_directories(./src)                                           # Current directory src
# include_directories(${CMAKE_SOURCE_DIR}/lib/boost)                   # Boost | pre-built boost/boost directory can be downloaded, as building boost is cumbersome
#                     ^                                                  ^
# include_directories(${CMAKE_SOURCE_DIR}/lib/libcURL/include)         # libcURL
#                     ^                                                  ^
###############################################
include_directories(./include)                                       # Current directory include
include_directories(./src)                                           # Current directory src

include_directories(${CMAKE_SOURCE_DIR}/src/Utils/include)           # Utils

include_directories(${CMAKE_SOURCE_DIR}/lib/boost)                   # Boost | pre-built boost/boost directory can be downloaded, as building boost is cumbersome

include_directories(${CMAKE_SOURCE_DIR}/lib/gumbo/include)           # gumbo
###############################################


###############################################
# 4. Resources
#              To be copied directly to bin.
#Example (copying whole directory):
# file(COPY "values" DESTINATION "${CMAKE_SOURCE_DIR}/bin")
#            ^                     ^
#
#Example (copying single file):
# configure_file(values/UserAgents.xml ${CMAKE_SOURCE_DIR}/bin/values/UserAgents.xml @COPYONLY)
#                ^                     ^
###############################################
#file(COPY "values" DESTINATION "${CMAKE_SOURCE_DIR}/bin")
###############################################


###############################################
# 5. Dependencies
#                External libraries (.dll/.lib etc)
#
#Example:
# ## Boost
# SET(BOOST_PROGRAM_OPTIONS_RELEASE ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/libboost_program_options-vc140-mt-1_64)    #Boost::Program_Options : Release
#     ^                             ^                                                                                  ^
# SET(BOOST_PROGRAM_OPTIONS_DEBUG ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/libboost_program_options-vc140-mt-gd-1_64)   #Boost::Program_Options : Debug
#     ^                           ^                                                                                    ^
# SET(BOOST_PROGRAM_OPTIONS optimized ${BOOST_PROGRAM_OPTIONS_RELEASE} debug ${BOOST_PROGRAM_OPTIONS_DEBUG})
#     ^                     ^                                          ^
# TARGET_LINK_LIBRARIES(${MODULE_NAME} ${BOOST_PROGRAM_OPTIONS})
#                                      ^
# 
# ## libcURL
# SET(CURLLIB ${CMAKE_SOURCE_DIR}/lib/libcURL/lib/libcurl)         #libcurl
#     ^       ^                                                     ^
# TARGET_LINK_LIBRARIES(${MODULE_NAME} ${CURLLIB})
#                                      ^
# add_custom_command(TARGET ${MODULE_NAME} POST_BUILD				# Adds a post-build event to MyTest                  #copy dll to bin
# 		COMMAND ${CMAKE_COMMAND} -E copy_if_different			    # which executes "cmake - E copy_if_different..."
#         "${CURLLIB}.dll"		                                    # <--this is in-file
#          ^
#         ${CMAKE_SOURCE_DIR}/bin)								    # <--this is out-file path
###############################################		
## Boost
SET(BOOST_LOG_RELEASE ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/boost_log-vc140-mt-1_64)    #Boost::Log : Release
SET(BOOST_LOG_DEBUG ${CMAKE_SOURCE_DIR}/lib/boost/stage/lib/boost_log-vc140-mt-gd-1_64)   #Boost::Log : Debug
SET(BOOST_LOG optimized ${BOOST_LOG_RELEASE} debug ${BOOST_LOG_DEBUG})
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${BOOST_LOG})

# Gumbo
SET(GUMBO_RELEASE ${CMAKE_SOURCE_DIR}/lib/gumbo/lib/gumbo_release)                #Gumbo : Release
SET(GUMBO_DEBUG ${CMAKE_SOURCE_DIR}/lib/gumbo/lib/gumbo_debug)                    #Gumbo : Debug
SET(BOOST_LOG optimized ${GUMBO_RELEASE} debug ${GUMBO_DEBUG})
TARGET_LINK_LIBRARIES(${MODULE_NAME} ${GUMBO_RELEASE})
###############################################