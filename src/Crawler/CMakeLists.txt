BuildDLL()
DependsOn(Boost.Log)
DependsOn(Utils)
DependsOn(Fetcher)
DependsOn(Parser)
DependsOn(Gumbo)
DependsOn(OpenCL)

BuildEXE(src/main.cpp)
DependsOn(Boost.Log)
DependsOn(Boost.ProgramOptions)
DependsOn(Utils)
DependsOn(Fetcher)
DependsOn(Parser)
DependsOn(Gumbo)
DependsOn(OpenCL)