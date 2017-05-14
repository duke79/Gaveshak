SET(MODULE_NAME libFetcher)

###############################################
file(GLOB MODULE_SRC
    "include/*.h"
    "src/*.cpp"
)
file(GLOB TinyXML2_SRC
    "${CMAKE_SOURCE_DIR}/lib/TinyXML2/*.h"
    "${CMAKE_SOURCE_DIR}/lib/TinyXML2/*.cpp"
)
include_directories(./include)                                                       # Current directory include
include_directories(./src)                                                           # Current directory src
include_directories(${CMAKE_SOURCE_DIR}/lib/TinyXML2)                                # TinyXML2
add_library(${MODULE_NAME} SHARED ${MODULE_SRC} ${TinyXML2_SRC})                     # Fetcher.dll
###############################################

CopyDir(values)

###############################################

DependsOn(Utils)
DependsOn(LibCurl)
DependsOn(Boost.Log)
DependsOn(Boost.ProgramOptions)
DependsOn(shlwapi)

###############################################
SET(MODULE_NAME Fetcher)
add_executable(${MODULE_NAME} src/main.cpp)         # Fetcher.exe
DependsOn(Fetcher)
DependsOn(Boost.Log)
DependsOn(Boost.ProgramOptions)