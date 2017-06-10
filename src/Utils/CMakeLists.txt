BuildDLL()
DependsOn(Boost.Log)
DependsOn(Boost.Regex)
DependsOn(Cassandra)

BuildEXE(src/main.cpp)
DependsOn(Boost.Log)
DependsOn(Boost.Regex)
DependsOn(Cassandra)