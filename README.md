# id Tech 3 Map Compiler
This project is a modified version of [Q3Map2](http://q3map2.robotrenegade.com/), which is a map compiler for games based on id Tech 3 engine, forked from [NetRadiant's q3map2 2.5.17n](https://gitlab.com/xonotic/netradiant/tree/225d4bba22a409bc1c375a2029d2162d336f0070).
All API changes, bug fixes and new features are listed in the [changelog](changelog.md).

## Wiki
The [wiki](https://github.com/id-tech-3-tools/map-compiler/wiki) page has detailed information about all switches map compiler currently supports.

## Scripts
The [scripts](https://github.com/id-tech-3-tools/map-compiler/tree/master/scripts) directory contains fully functional batch scripts to do compiling, decompiling and model conversion work.

## Precompiled binaries
Windows binaries can be found under [releases](https://github.com/id-tech-3-tools/map-compiler/releases/) page. 

## Contributing
You are free to make feature requests/bug reports under issues, as well as pull requests of course. More information can be found [here](CONTRIBUTING.md).

## External dependencies 
All required dependencies are shipped with the project.  
[assetsys](https://github.com/mattiasgustavsson/libs/blob/master/docs/assetsys.md) loads binary assets into virtual file system.  
The library used in a project contains [custom changes](https://github.com/isRyven/libs). MIT License.  
[Catch2](https://github.com/catchorg/Catch2) test framework for unit-tests, TDD and BDD. Boost Software License.  
[filesystem](https://github.com/wjakob/filesystem) a simple class for manipulating paths on Linux/Windows/Mac OS. BSD-3 License.  
[jpeg-compressor](https://github.com/richgel999/jpeg-compressor) jpeg compression/decompression lib. Public domain.  
[lodepng](https://github.com/lvandeve/lodepng) PNG encoder and decoder. zlib License.  
[msdirent.h](https://svn.apache.org/repos/asf/avro/trunk/lang/c/tests/msdirent.h) dirent API for MS Windows. MIT License.  
[tinyformat](https://github.com/c42f/tinyformat) type safe printf replacement library. Boost Software License.

## Compiling binaries

### Compiling on linux
```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE .. && make
```

### Compiling on Windows
Several options are available.

#### Visual Studio 2017
In VS2017 you can natively open up the CMake project folder: `File > Open > CMake`, and compile the project using CMake menu.

#### Compile from command line
```
mkdir build
cd build
cmake ..
cmake --build . --target mapcompiler --config Release
```

#### Generate Visual Studio solution
```
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017"
```
Open up `sln` in Visual Studio and build solution from there.

# Original Q3Map2 License 
```
Copyright (C) 1999-2007 id Software, Inc. and contributors.
For a list of contributors, see the accompanying CONTRIBUTORS file.

This file is part of GtkRadiant.

GtkRadiant is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

GtkRadiant is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GtkRadiant; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

----------------------------------------------------------------------------------

This code has been altered significantly from its original form, to support
several games based on the Quake III Arena engine, in the form of "Q3Map2."
```

