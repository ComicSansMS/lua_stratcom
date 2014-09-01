------------------------------------------------------------------------------
 lua_stratcom - Lua 5.2 language bindings for libstratcom
------------------------------------------------------------------------------

 -- Getting Started --

lua_stratcom allows interacting with the Microsoft SideWinder
Strategic Commander from within Lua scripts.

See the enclosed test.lua for a simple example.

lua_stratcom provides a Lua module which implements a thin wrapper
around the original libstratcom.
Take a look at the libstratcom API documentation to get started.

 -- Compilation --
 
You need an installed version of libstratcom to build lua_stratcom:
    https://github.com/ComicSansMS/libstratcom

lua_stratcom uses CMake (http://cmake.org/) for building.

CMake will generate a build environment suited to your platform. This might
be Makefiles on a Unix-like system or Visual Studio project files on a
Windows system.

For example, to build and install the library on a *nix system, it is
usually sufficient to run:

    cd <source_directory>
    cd build
    cmake ..
    make

 -- License --

The libstratcom library can be used under the MIT/X11 license.
It requires the HIDAPI library as a dependency, which can be used under the
terms of the BSD license.

Lua is released under the Lua license, available at
http://www.lua.org/license.html

See the enclosed LICENSE.txt file for details.
