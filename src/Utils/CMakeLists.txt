SET(MODULE_NAME Utils)

AddDLL(${MODULE_NAME})

DependsOn(Boost.Log)
DependsOn(Boost.Regex)