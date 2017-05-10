SET(MODULE_NAME Gaveshak)

AddEXE(${MODULE_NAME} src/main.cpp)

DependsOn(Utils)
DependsOn(Fetcher)
DependsOn(Parser)
DependsOn(Boost.ProgramOptions)
DependsOn(Boost.Log)
DependsOn(shlwapi)
DependsOn(Gumbo)