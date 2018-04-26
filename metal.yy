%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {Metal}
%define parser_class_name {Parser}
%language "C++"
			
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
    FunctionDeclaration * functionDeclaration;
    int intValue;
    Program * program;
    std::string * string;
    Struct * strct;
    VariableAttribute * variableAttribute;
    VariableDeclaration * variableDeclaration;
    VariableList * variableList;
    
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
%token	<intValue>    INT_VALUE
%token	<string>      VARIABLE_ATTRIBUTE_INDEX
%token                USING_NAMESPACE
%token                BEGIN_CURLY_BRACKET
%token                END_CURLY_BRACKET
%token                BEGIN_BRACKET
%token                END_BRACKET
%token                BEGIN_DOUBLE_SQUARE_BRACKET
%token                END_DOUBLE_SQUARE_BRACKET

%type	<block> declaration_list
%type	<declaration>	 declaration
%type	<functionDeclaration> function_declaration
%type	<program> translation_unit
%type	<string>	identifier
%type	<strct>		struct
%type	<variableAttribute> variable_attribute
%type	<variableDeclaration> variable_declaration
%type	<variableList> variable_list
%locations

%%
translation_unit: declaration_list { _root = new Program($1); $$ = _root; delete $1; }
		;

struct: STRUCT identifier BEGIN_CURLY_BRACKET declaration_list END_CURLY_BRACKET { $$ = new Struct(*$2); $$->_block = *$4; delete $4; }
		;

declaration_list: /* empty */ { $$ = new Block() ; }
	|	declaration_list declaration { $$->_nodes.push_back($2);}
	|	declaration { $$ = new Block() ;  $$->_nodes.push_back($1); }
		;

variable_attribute: BEGIN_DOUBLE_SQUARE_BRACKET identifier BEGIN_BRACKET INT_VALUE END_BRACKET END_DOUBLE_SQUARE_BRACKET { $$ = new VariableAttribute(*$2,$4); }
	|	BEGIN_DOUBLE_SQUARE_BRACKET identifier END_DOUBLE_SQUARE_BRACKET { $$ = new VariableAttribute(*$2); }	
	;

variable_declaration: identifier identifier { $$ = new VariableDeclaration(*$1, *$2); }
	| 	identifier identifier variable_attribute { $$ = new VariableDeclaration(*$1, *$2, $3); }
		;

variable_list: /* empty */ { $$ = new VariableList(); }
	|	variable_list variable_declaration { $$->_variableDeclarations.push_back($2);}
	|	variable_declaration { $$ = new VariableList() ;  $$->_variableDeclarations.push_back($1); }
		;

function_declaration : identifier identifier identifier BEGIN_BRACKET variable_list END_BRACKET BEGIN_CURLY_BRACKET declaration_list END_CURLY_BRACKET { $$ = new FunctionDeclaration(*$1, *$2, *$3, $5, $8) ; }

declaration: USING_NAMESPACE identifier SEMICOLON {  $$ = new UsingDeclaration(*$2); }
	| 	struct SEMICOLON { $$ = $1; }
	| 	variable_declaration SEMICOLON { $$ = $1; }
	|	function_declaration { $$ = $1; }
		;


identifier: IDENTIFIER { $$ = new std::string(*$1); delete $1; }
		
%%
		
