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

}

%define api.value.type variant
%define parse.assert
			  
%token		      END               0 "end of file"
%token		      EOL                 "end of line"
%token		      BOOL		      
%token		      FLOAT
%token		      FLOAT2
%token		      FLOAT3
%token		      FLOAT4
%token		      UCHAR
%token		      UCHAR2
%token		      UCHAR3
%token		      UCHAR4
%token		      HALF 	
%token		      DOUBLE	
%token		      INT	
%token		      STRING
%token 		      SKIP
%token 		      STRUCT
%token		      SEMICOLON
%token <std::string>  IDENTIFIER
%token                USING_NAMESPACE
%token                BEGIN_CURLY_BRACKET
%token                END_CURLY_BRACKET
%token                BEGIN_BRACKET
%token                END_BRACKET
%token                BEGIN_DOUBLE_SQUARE_BRACKET
%token                END_DOUBLE_SQUARE_BRACKET

%locations

%%
translation_unit: declaration_list

struct: STRUCT IDENTIFIER BEGIN_CURLY_BRACKET struct_content END_CURLY_BRACKET SEMICOLON

struct_content: /* empty */
	| EOL struct_content
	| type struct_content	
				
declaration_list: declaration declaration_list
	| struct declaration_list
	| type declaration_list
	| EOL declaration_list
	| END	

type: FLOAT
	| FLOAT2	
	| FLOAT3	
	| FLOAT4
	| UCHAR
	| UCHAR2
	| UCHAR3
	| UCHAR4
		
declaration: USING_NAMESPACE IDENTIFIER SEMICOLON
		
%%
		
void Metal::Parser::error(const location_type &line, const std::string &err)
{
    std::cerr << line << ": " << err << std::endl; 
}
