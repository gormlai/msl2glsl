# msl2glsl
Parses MSL and spits out GLSL

*Contributors*

Gorm Lai <lai.gorm@gmail.com>

*Dependencies*

** Linux ** 
To build the following programs are needed in your path:
* Bison (v 3+)
* Flex
* Meson 
* A C++14 capable compiler

** Windows **

* Visual Studio 2017 Community Edition (It may work with other compilers but this has been tested)
* Clang (tested with Clang 6.0 for Windows (clang for Windows can be found here https://llvm.org/builds/, and a guide for setting it up visual studio can be found here https://github.com/boostorg/hana/wiki/Setting-up-Clang-on-Windows)
* WinFlexBison (check the readme for how to setup the custom build rules)

*Building*



*Inspirational sources*

* http://www.jonathanbeard.io/tutorials/FlexBisonC++.html was essential in refreshing my uni knowledge on writing compilers and parsers.
* http://www.quut.com/c/ was essential in for inspiration in building a syntax as most ECMA inspired languages look similar.
* http://www.lysator.liu.se/c/ANSI-C-grammar-y.html the ANCI C yacc grammar (1985)
* http://www.lysator.liu.se/c/ANSI-C-grammar-l.html the ANCI C grammar, lex specification (1985)
* http://www.nongnu.org/hcb/ C++ BNF Grammer
* http://www.fuglgame.com - the game that started it all

*Background*



*Improvements*

* Make fully compliant with Metal syntax (Metal is based on C++14, while this is more based on a C85 standard)
* Argument parsing
** Argument for not adding version tag
* Make location keyword on in/out optional
* Make uniform blocks optional
* Fix how initializers are handled as they only work with certain constructs
* Bug with insertion of empty statements (;) in if-else after statements blocks
* Automatically calculate version tag
* Add support for atan2
* Fix bug with ; created after statement blocks
* Deal with incompatibilities such as being able to modify input on msl, but not on glsl
