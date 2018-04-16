# id Tech 3 Map Compiler
This project aims to be an all-in app for compiling maps for games based on id Tech 3 engine. For now, the development is focused towards W:ET and q3map2. At it's current state, the project is just a modified version of [q3map2](http://q3map2.robotrenegade.com/), forked from [NetRadiant's q3map2 2.5.17n](https://gitlab.com/xonotic/netradiant/tree/225d4bba22a409bc1c375a2029d2162d336f0070).

## Wiki
The [wiki](https://github.com/isRyven/map-compiler/wiki) page has detailed information about all options map compiler currently supports.

## Precompiled binaries
Windows binaries can be found under [releases](https://github.com/isRyven/map-compiler/releases/) page. 

## Changelog
All API changes and new features can be found in our [changelog](changelog.md).

## Contributing
You are free to make feature requests/bug reports under issues, as well as pull requests of course. More information can be found [here](CONTRIBUTING.md).

## External dependencies 
All required dependencies are shipped with the project.  
[Argh!](https://github.com/adishavit/argh) a minimalist argument handler. BSD-3 License.  
[assetsys](https://github.com/mattiasgustavsson/libs/blob/master/docs/assetsys.md) loads binary assets into virtual file system.  
The library used in a project contains [custom changes](https://github.com/isRyven/libs). MIT License.  
[Catch2](https://github.com/catchorg/Catch2) test framework for unit-tests, TDD and BDD. Boost Software License.  
[filesystem](https://github.com/wjakob/filesystem) a simple class for manipulating paths on Linux/Windows/Mac OS. BSD-3 License.  
[jpeg-compressor](https://github.com/richgel999/jpeg-compressor) jpeg compression/decompression lib. Public domain.  
[lodepng](https://github.com/lvandeve/lodepng) PNG encoder and decoder. zlib License.  
[Lua](https://github.com/lua/lua) efficient, lightweight, embeddable scripting language. MIT License.  
[msdirent.h](https://svn.apache.org/repos/asf/avro/trunk/lang/c/tests/msdirent.h) dirent API for MS Windows. MIT License.  
[tinyformat](https://github.com/c42f/tinyformat) type safe printf replacement library. Boost Software License.

## Compiling
The project currently only compiles under Windows x64 using Visual Studio 2015+.
### Requirements
* [Visual Studio](https://www.visualstudio.com/vs/community/), community edition is sufficient.
* [git](https://git-scm.com/) version control system.
### Instructions
1. [Git clone](https://help.github.com/articles/cloning-a-repository/) the project.
2. Open up `map-compiler.sln` in Visual Studio.
3. [Compile Solution](https://msdn.microsoft.com/en-us/library/5tdasz7h.aspx).
4. Compiled binaries can be found in `build` directory of the project root.
