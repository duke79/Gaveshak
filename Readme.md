## Build & Install
### Dependencies
#### Boost
* Install boost to lib/boost
 https://sourceforge.net/projects/boost/files/boost-binaries/
* Run boostrap.bat
* Run b2.exe

#### CMake 
* Download and install **CMake** (minimum version 2.6)

### Build
* Create a directory named **build** (or any other name) in the root
* Change to **build**
* Run `cmake ..` (use *-G* option to force *'Visual Studio 14'* if required) **
* Open *Project.sln* with Visual Studio
* Set *Gaveshak* as Startup Project
* Hit F5 (alternatively 

** *Alternatively GNU make can also be used to build directly from command line. 
But somehow copy command doesn't work, so all the required libraries need to be manually copied to Root/bin first.*