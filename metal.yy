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

Program * _root = nullptr;

}

//			%define api.value.type variant
%define parse.assert

%start translation_unit
			
%union
{
    Program *program;
    DeclarationList *declarationList;
    Declaration *declaration;
    std::string * string;
}
			  
%token		      END               0 "end of file"
%token		      EOL                 "end of line"
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

%type	<program>	translation_unit
%type	<declarationList> declaration_list
%type	<declaration>	 declaration
%locations

%%
translation_unit: declaration_list { _root = new Program($1);  }
		;

struct: STRUCT IDENTIFIER BEGIN_CURLY_BRACKET struct_content END_CURLY_BRACKET SEMICOLON

struct_content: /* empty */
	| EOL struct_content
	| declaration_variable struct_content	
				
declaration_list: declaration declaration_list { $$ = new DeclarationList(); $$->_nodes.push_back($1);}
	| struct declaration_list
	| type declaration_list
	| EOL declaration_list
	| END
		;

declaration_variable: type IDENTIFIER SEMICOLON
		
type: TYPE_FLOAT
	| TYPE_FLOAT2	
	| TYPE_FLOAT3	
	| TYPE_FLOAT4
	| TYPE_UCHAR
	| TYPE_UCHAR2
	| TYPE_UCHAR3
	| TYPE_UCHAR4
		
declaration: USING_NAMESPACE IDENTIFIER SEMICOLON { $$ = new UsingDeclaration(*$2); }
		;

		
%%
		
void Metal::Parser::error(const location_type &line, const std::string &err)
{
    std::cerr << line << ": " << err << std::endl; 
}
