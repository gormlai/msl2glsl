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
			
#define YYERROR_VERBOSE
			
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
%token 		      STRUCT
%token		      SEMICOLON
%token                IDENTIFIER
%token                USING_NAMESPACE
%token                BEGIN_CURLY_BRACKET
%token                END_CURLY_BRACKET

%locations

%%
translation_unit: declaration_list

declaration_list: declaration declaration_list
	| SEMICOLON
		
declaration: USING_NAMESPACE IDENTIFIER
	| EOL
	| END
	| SEMICOLON



		
%%
		
void Metal::Parser::error(const location_type &line, const std::string &err)
{
    std::cerr << line << ": " << err << std::endl; 
}
