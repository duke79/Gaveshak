## Build & Install
### Dependencies

#### Boost *(1.64.0)*
* Install boost to lib/boost
 https://sourceforge.net/projects/boost/files/boost-binaries/
* Run <code>boostrap.bat</code>
* Run <code>b2.exe link=shared</code>

#### CMake 
* Download and install **CMake** (minimum version 2.6)

#### cURL *(curl-7.54.0)*
[#cmake#](http://stackoverflow.com/questions/31705338/libcurl-in-visual-studio-2013-static-linker-errors-even-after-including-necessar)
* Download latest code from https://github.com/bagder/curl
* Clone and navigate to that directory. Run the following commands 

  <code>
  mkdir build  
  cd build  
  cmake ..
  </code>

* This builds a Visual Studio Solution. Open the solution and build it.
* Following files are generated in the following path:
  - path_to_pulled_code\build\lib\Release\libcurl.dll
  - path_to_pulled_code\build\lib\Release\libcurl_imp.exp
  - path_to_pulled_code\build\lib\Release\libcurl_imp.lib


[#nmake#](https://gist.github.com/grahamcrowell/2d3f069d89c78305f8001817c911345e)
* Change to /winbuild
* Run the following command
<code>nmake /f makefile.vc ENABLE_WINSSL=yes mode=dll MACHINE=x86 VC=14</code>
* nmake is available in VS binaries. *(Microsoft Visual Studio 14.0\VC\bin)*
  - builds/libcurl-vc14-x86-release-dll-ipv6-sspi-winssl/bin/libcurl.dll
  - builds/libcurl-vc14-x86-release-dll-ipv6-sspi-winssl/bin/curl.exe
  - builds/libcurl-vc14-x86-release-dll-ipv6-sspi-winssl/lib/libcurl.lib
  - builds/libcurl-vc14-x86-release-dll-ipv6-sspi-winssl/lib/libcurl.exp

#### Gumbo-Parser (0.10.1)
* Download repository from https://github.com/google/gumbo-parser
* Open VS project in  **/visualc** and build it to get *gumbo.lib*
  - Build two variants of **gumbo.lib**, ie. *debug & release* { gumbo_debug.lib & gumbo_release.lib }
  - Gumbo doesn't export symbols, so DLL is not usable
  
* Copy these libraries to Gaveshak/lib/gumbo/lib
* COpy all .h files from gumbo/src to Gaveshak/lib/gumbo/include

#### Gumbo-Query
* Download repository from https://github.com/lazytiger/gumbo-query
* Copy the headers to *Gaveshak/src/Parser/include*
* Copy the .cpp files to *Gaveshak/src/Parser/src*
* Export all the classes of Gumbo-Query (Document, Node, ...)

##### #Copy files#
* Put these generated files in **lib/libcURL/lib**
* Copy headers from 
  - *path_to_pulled_code/include/curl* 
  - to **lib/libcURL/include/curl**

### Build
* Create a directory named **build** (or any other name) in the root
* Change to **build**
* Run `cmake ..` (use *-G* option to force *'Visual Studio 14'* if required) **
* Open *Project.sln* with Visual Studio
* Set *Gaveshak* as Startup Project
* Hit F5

** *Alternatively GNU make can also be used to build directly from command line. 
But somehow copy command doesn't work, so all the required libraries need to be manually copied to Root/bin first.*