# msl2glsl
Parses MSL and spits out GLSL

*Contributors*
Gorm Lai <lai.gorm@gmail.com>

*Building*

To build the following programs are needed in your path:
* Bison (v 3+)
* Flex
* Meson
* A C++14 capable compiler


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
* Make location keyword on in/out optional
* Make uniform blocks optional
