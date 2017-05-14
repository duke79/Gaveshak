BuildDLL()

DependsOn(Boost.Log)
DependsOn(Boost.Regex)

BuildEXE(src/main.cpp)
DependsOn(Boost.Log)
DependsOn(Boost.Regex)