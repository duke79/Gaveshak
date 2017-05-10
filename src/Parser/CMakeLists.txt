SET(MODULE_NAME Parser)

AddDLL(${MODULE_NAME})

DependsOn(Utils)
DependsOn(Boost.Log)
DependsOn(Gumbo)