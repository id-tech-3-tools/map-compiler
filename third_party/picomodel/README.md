# PicoModel Library
This is a library used in GtkRadiant(and its derivatives) and Q3Map2 to parse various generic model formats, including Quake2, Quake3 and RTCW models. It was originaly developed by Randy Reddig(ydnar) and seaw0lf. The library have been then modified several times by [NetRadiant](https://gitlab.com/xonotic/netradiant) contributors. All changes are preserved.

## Supported model formats
* MD3 (quake3)
* 3DS (3ds Max)
* ASE (ASCII Scene Exporter)
* MS3D (milkshape3d)
* MDC (RTCW)
* MD2 (quake2)
* FM (heretic2)
* LWO (lightwave object)
* Terrain (picoterrain object)
* OBJ (wavefront object)
* MDL (quake1)

## Build
This repository contains build files to compile `libpicomodel` static library and tests executable, that will reside in `build` directory after successful compilation.

### Compiling on linux
```
mkdir build && cd build
cmake .. && make
```

### Compiling on Windows
There are several options available.

#### Visual Studio 2017
In VS2017 you can natively open up the CMake project folder: `File > Open > CMake`.

#### Compile from command line
```
mkdir build
cd build
cmake ..
cmake --build . --target picomodel --config Release
```

#### Visual Studio solution file
```
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017"
```
Open up `sln` in Visual Studio and build solution from there.

# Original License
```
PicoModel Library

Copyright (c) 2003, Randy Reddig & seaw0lf
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

Neither the names of the copyright holders nor the names of its contributors may
be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
