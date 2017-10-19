# Build & Install
## Dependencies

### 1. Boost *(1.64.0)*
Boost is already compiled, zipped and a part of this repo. To use it directly-
* Unzip boost from ***zip/boost_1_64_0_SHARED.7z*** into lib/boost

We currently use boost DLLs for dynamic linking. If static linking is required *zip\boost_1_64_0.7z* can be used.

To use a newer version of boost :
* [Download](https://sourceforge.net/projects/boost/files/boost-binaries/) boost and install it to lib/boost
* Run the following commands -
```
boostrap.bat
b2.exe link=shared
```

### 2. CMake 
* Download and install **CMake** (minimum version 2.6)

### 3. cURL *(curl-7.54.0)*
*{Pre-installed} - We may skip this step unless a newer version of cURL is required.*

There are two ways to build cURL - cmake & nmake.

[#cmake#](http://stackoverflow.com/questions/31705338/libcurl-in-visual-studio-2013-static-linker-errors-even-after-including-necessar)
* Download latest code from https://github.com/bagder/curl
* Clone and navigate to that directory. Run the following commands 
```  
mkdir build  
cd build  
cmake ..
```

* This builds a Visual Studio Solution. Open the solution and build it.
* Following files are generated :
    * build\lib\Release\libcurl.dll
    * build\lib\Release\libcurl_imp.exp
    * build\lib\Release\libcurl_imp.lib


[#nmake#](https://gist.github.com/grahamcrowell/2d3f069d89c78305f8001817c911345e)
* Change to /winbuild
* Run the following command
```
nmake /f makefile.vc ENABLE_WINSSL=yes mode=dll MACHINE=x86 VC=14
```
nmake is available in VS binaries. *(Microsoft Visual Studio 14.0\VC\bin)*
  - builds/libcurl-vc14-x86-release-dll-ipv6-sspi-winssl/bin/libcurl.dll
  - builds/libcurl-vc14-x86-release-dll-ipv6-sspi-winssl/bin/curl.exe
  - builds/libcurl-vc14-x86-release-dll-ipv6-sspi-winssl/lib/libcurl.lib
  - builds/libcurl-vc14-x86-release-dll-ipv6-sspi-winssl/lib/libcurl.exp

#### #Copy files#
* Put these generated files in **lib/libcURL/lib** (exe is not required :P)
* Copy headers from 
    * *path_to_pulled_code/include/curl* 
    * to **lib/libcURL/include/curl**

### 4. Gumbo-Parser (0.10.1)
*{Pre-installed} - We may skip this step unless a newer version of Gumbo-Parser is required.*
* Download repository from https://github.com/google/gumbo-parser
* Open VS project in  **/visualc** and build it to get *gumbo.lib*
* Build two variants of **gumbo.lib**, ie. *debug & release* { gumbo_debug.lib & gumbo_release.lib }. Gumbo doesn't export symbols, so DLL is not usable

* Copy these libraries to Gaveshak/lib/gumbo/lib
* Copy all .h files from gumbo/src to Gaveshak/lib/gumbo/include

### 5. Gumbo-Query
*{Pre-installed} - We may skip this step unless a newer version of Gumbo-Query is required.*
* Download repository from https://github.com/lazytiger/gumbo-query
* Copy the headers to *Gaveshak/src/Parser/include*
* Copy the .cpp files to *Gaveshak/src/Parser/src*
* Export all the classes of Gumbo-Query (Document, Node, ...)

### 6. Storage
#### HBase
* [Cygwin_Hbase](https://hbase.apache.org/cygwin.html)

#### Cassandra
* [Docs](https://datastax.github.io/cpp-driver/topics/basics/)
* [Installation](https://docs.nomagic.com/display/CEDW183/Installing+and+configuring+Cassandra+on+Windows)
* [Getting started](https://www.datastax.com/2012/01/getting-started-with-apache-cassandra-on-windows-the-easy-way)
* [Cassandra - DataStax downloads](http://downloads.datastax.com/community/)
    * datastax-community-64bit_3.0.9_2.msi
* Windows binaries *(32-bit)* for [Cpp-Driver](https://github.com/datastax/cpp-driver) can be downloaded from [here](http://downloads.datastax.com/cpp-driver/windows/) along with dependencies.
    * cassandra-cpp-driver-2.7.0-win32-msvc140
* [DevCenter](https://academy.datastax.com/downloads/download-previous-versions#dl-opscenter)
    * DevCenter-1.5.0-win-x86_64.zip
* Configuration
    * Open ports in firewall (22-62000). Inbound & Outbound both.
    * Cassandra.yaml
        * cluster_name must be same for all the nodes
        * rpc_address & listen_address must be IP of current machine in network
        * seeds must be a list of servers which provide info to a new node about cluster. (Maybe one or two machines which are treated as host must be enough.)
    * Create a [cqlshrc](http://docs.datastax.com/en/cql/3.1/cql/cql_reference/cqlshrc.html) with hostname set to current IP.
    * Add ../apache-cassandra/bin to path.
    * Run the following command to test the nodes status -
      `nodetool status`
    * Multiple node clusters [reference1](http://docs.datastax.com/en/cassandra/2.1/cassandra/initialize/initializeSingleDS.html)
      & [reference2](http://zcourts.com/2011/07/09/setting-up-a-multi-node-cassandra-cluster-on-a-single-windows-machine/#sthash.RfPaZpri.JizaCST6.dpbs) if required

### 6. Testing
#### Google Test
Visual studio solution setting for **gtest** before building :
| VS Setting          | Value                                 |
| ------------------- | ------------------------------------- |
| **Runtime Library** | ***Multi-threaded Debug DLL (/MDd)*** |


## Build
In the root directory -
```
mkdir build
cd build
cmake .. -Wno-dev
```
This builds a Visual Studio Solution. Open the solution and build it.

# Planning & Information
### ToDo
* [Design/Architecture] (https://chromium.googlesource.com/chromium/src/+/master/styleguide/styleguide.md)
* Testing
  * GoogleTest : https://github.com/google/googletest
* Smart Pointers
  * http://www.chromium.org/developers/smart-pointer-guidelines	
* What need for Globals.h?
  * Although environment variables in future maybe!
* EXE for modules
  * Maybe useful in testing too
  * Classes must be testing units, instead of modules
    * http://softwaretestingfundamentals.com/unit-testing/
* Fetcher
  * Download in parts
  * Cookies to be stored in local files?
  * User agents sortable/categorized by platform and browser
    * May use a list of classes/structs representing a useragent
* Crawler
  * Extract "text" from the page before storing. (or not? Google stores original pages!)

## Crawler
[*How to build a crawler-Quora*](https://www.quora.com/How-can-I-build-a-web-crawler-from-scratch)

A bare minimum crawler needs at least these components:
* **Extractor:** Minimal support to extract URL from page like anchor links.
* **Duplicate Eliminator:** To make sure same content is not extracted twice unintentionally. Consider it as a set based data structure.
* **URL Frontier:** To prioritize URL that has to fetched and parsed. Consider it as a priority queue
* **Datastore:** To store retrieve pages and URL and other meta data.
* [Min. Delay](https://en.wikipedia.org/robots.txt)
* [Bot ID](https://www.quora.com/How-does-Google-crawl-Facebook-or-Quora-data)

### Crawler - ToDo
* Relative path
* MyID?
* Bad URLs problem?
* Robots.txt
* Frequent proxy switch
* Switch user agents once in a while (use the famous ones)
* URL parsing (domain, http/https/...?, file-extension ...)
* Delay before fetching from same domain
* Avoid traps
* Max file size limit even if size is not know in advance

## Scraping/Parsing

## Indexing

## Ranking

## Query Processing

## Search Algorithm
* https://en.wikipedia.org/wiki/Inverted_index
* https://en.wikipedia.org/wiki/Vector_space_model

## Data management
* **HBASE**
* http://stackoverflow.com/questions/362956/what-database-does-google-use
* https://en.wikipedia.org/wiki/Spanner_(database)
* https://en.wikipedia.org/wiki/Bigtable
* https://research.google.com/archive/bigtable.html

## Distributed/Parallel Processing

## Caching

## Load balancing

## Redundancy

## Analytics

## Links 
* https://en.wikipedia.org/wiki/Semantic_search
* https://www.quora.com/I-am-planning-to-make-a-small-scale-search-engine-on-my-local-system-from-where-should-I-start
* https://www.quora.com/Will-Google-be-replaced-If-so-how
* http://kameir.com/future-of-google/
* https://www.quora.com/What-could-eventually-replace-Google-search
* https://softwareengineering.stackexchange.com/questions/38324/how-would-you-implement-google-search