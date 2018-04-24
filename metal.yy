%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {Metal}
%define parser_class_name {Parser}

%code requires {

#include "Types.h"
    
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

    //Program * _root = nullptr;

}

//			%define api.value.type variant
%define parse.assert

%start translation_unit
			
%union
{
    Block * block;
    Declaration *declaration;
    std::string * string;
    Struct * strct;
}
			  
%token		      TYPE_BOOL		      
%token		      TYPE_FLOAT
%token		      TYPE_FLOAT2
%token		      TYPE_FLOAT3
%token		      TYPE_FLOAT4
%token		      TYPE_UCHAR
%token		      TYPE_UCHAR2
%token		      TYPE_UCHAR3
%token		      TYPE_UCHAR4
%token		      TYPE_HALF 	
%token		      TYPE_DOUBLE	
%token		      TYPE_INT	
%token		      TYPE_STRING
%token 		      SKIP
%token 		      STRUCT
%token		      SEMICOLON
%token	<string>      IDENTIFIER
%token                USING_NAMESPACE
%token                BEGIN_CURLY_BRACKET
%token                END_CURLY_BRACKET
%token                BEGIN_BRACKET
%token                END_BRACKET
%token                BEGIN_DOUBLE_SQUARE_BRACKET
%token                END_DOUBLE_SQUARE_BRACKET

%type	<block> declaration_list translation_unit
%type	<declaration>	 declaration
%type	<strct>		struct
%type	<string>	identifier
%locations

%%
translation_unit: declaration_list { printf("0\n"); _root = $1;  }
		;

struct: STRUCT identifier BEGIN_CURLY_BRACKET struct_content END_CURLY_BRACKET SEMICOLON { $$ = new Struct(*$2); }
		;

struct_content: declaration_variable
	| struct_content declaration_variable 
		;
				
declaration_list:  declaration_list declaration { $$->_nodes.push_back($2);}
		       |	declaration { $$ = new Block() ;  $$->_nodes.push_back($1); }
		;

declaration_variable: type identifier SEMICOLON { }

type: TYPE_FLOAT
	| TYPE_FLOAT2	
	| TYPE_FLOAT3	
	| TYPE_FLOAT4
	| TYPE_UCHAR
	| TYPE_UCHAR2
	| TYPE_UCHAR3
	| TYPE_UCHAR4
		;

declaration: USING_NAMESPACE identifier SEMICOLON {  $$ = new UsingDeclaration(*$2); }
		 |	 struct { $$ = $1; }
		;

identifier: IDENTIFIER { $$ = new std::string(*$1); delete $1; }
		
%%
		
void Metal::Parser::error(const location_type &line, const std::string &err)
{
    std::cerr << line << ": " << err << std::endl; 
}
