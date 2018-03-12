%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {Metal}
%define parser_class_name {Parser}

%code requires {
namespace Metal {
class Driver;
class Scanner;
}
			
#if !defined YY_NULLPTR
#define YY_NULLPTR nullptr
#endif
			
}

%parse-param { Scanner &scanner }
%parse-param { Driver &driver}

%code {
#include <iostream>
#include <cstdlib>
#include <fstream>
    
#include "Driver.h"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert
			  
%token		      END               0 "end of file"
%token		      EOL                 "end of line"
%token		      BOOL		      
%token	<float>	      FLOAT
%token	<float>	      HALF 	
%token	<double>      DOUBLE	
%token	<int>	      INT	
%token	<std::string> STRING
%token 		      SKIP

%locations

%%
start: /* empty */
%%
		
void Metal::Parser::error(const location_type &line, const std::string &err)
{
    std::cerr << line << ": " << err << std::endl; 
}
