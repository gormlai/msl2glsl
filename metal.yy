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
    BufferDescriptor * bufferDescriptor;
    Expression * expression;
    FunctionDeclaration * functionDeclaration;
    float halfValue;
    float floatValue;
    double doubleValue;
    int intValue;
    Program * program;
    ReservedToken reservedToken;
    std::string * string;
    Statement * statement;
    Struct * strct;
    VariableAttribute * variableAttribute;
    VariableDeclaration * variableDeclaration;
    VariableList * variableList;
    VariableDeclaration::Qualifier qualifier;
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
%token		      COMMA
%token                DOT
%token                ASSIGN
%token                PLUS
%token                MINUS
%token                STAR
%token                AMPERSAND
%token		      FORWARD_SLASH
%token		      RETURN
%token	<qualifier>   CONSTANT
%token	<string>      IDENTIFIER
%token	<intValue>    INT_VALUE
%token	<halfValue>   HALF_VALUE
%token	<floatValue>  FLOAT_VALUE
%token	<doubleValue> DOUBLE_VALUE
%token	<string>      VARIABLE_ATTRIBUTE_INDEX
%token                USING_NAMESPACE
%token                BEGIN_CURLY_BRACKET
%token                END_CURLY_BRACKET
%token                BEGIN_BRACKET
%token                END_BRACKET
%token                BEGIN_DOUBLE_SQUARE_BRACKET
%token                END_DOUBLE_SQUARE_BRACKET
%token                DOUBLE_COLON
%token                ACCESS
%token                LESS_THAN
%token                GREATER_THAN

%type	<block> statements
%type	<statement>	 statement
%type	<functionDeclaration> function_declaration
%type	<program> translation_unit
%type	<string>	identifier
%type	<qualifier>	qualifier
%type	<strct>		struct
%type	<variableAttribute> variable_attribute
%type	<variableDeclaration> variable_declaration
%type	<variableList> variable_list
%type	<reservedToken>	 reserved_token
%type	<bufferDescriptor> buffer_descriptor
%type	<expression>   expression
%type	<expression>   expression0
%type	<expression>   expression1
%type	<expression>   constant
%locations

%%
translation_unit: statements { _root = new Program($1); $$ = _root; delete $1; }
		;

struct: STRUCT identifier BEGIN_CURLY_BRACKET statements END_CURLY_BRACKET { $$ = new Struct(*$2); $$->_block = *$4; delete $4; }
		;

statements: statements statement { $$->_nodes.push_back($2);}
	|	statement { $$ = new Block() ;  $$->_nodes.push_back($1); }
		;

variable_attribute: /* empty */ { $$ = nullptr; }

	|	BEGIN_DOUBLE_SQUARE_BRACKET identifier BEGIN_BRACKET INT_VALUE END_BRACKET END_DOUBLE_SQUARE_BRACKET { $$ = new VariableAttribute(*$2,$4); }
	|	BEGIN_DOUBLE_SQUARE_BRACKET identifier END_DOUBLE_SQUARE_BRACKET { $$ = new VariableAttribute(*$2); }	
	;

buffer_descriptor:
		      LESS_THAN identifier COMMA ACCESS DOUBLE_COLON identifier GREATER_THAN { $$ = new BufferDescriptor(*$2, *$6); }
	;

qualifier: 	CONSTANT { $$ = VariableDeclaration::Qualifier::Constant; }
	;

reserved_token:
	       AMPERSAND { $$ = ReservedToken::Star; }
//			|      '*' { $$ = ReservedToken::Ampersand; }
	;

variable_declaration:
	qualifier identifier reserved_token identifier variable_attribute { $$ = new VariableDeclaration($1, *$2, nullptr, $3, *$4, $5); }
|	identifier identifier variable_attribute { $$ = new VariableDeclaration(VariableDeclaration::Qualifier::None, *$1, nullptr, ReservedToken::None, *$2, $3); }
|	qualifier identifier identifier variable_attribute { $$ = new VariableDeclaration($1, *$2, nullptr, ReservedToken::None, *$3, $4); }
|	identifier reserved_token identifier variable_attribute { $$ = new VariableDeclaration(VariableDeclaration::Qualifier::None, *$1, nullptr, $2, *$3, $4); }
|	qualifier identifier buffer_descriptor reserved_token identifier variable_attribute { $$ = new VariableDeclaration($1, *$2, $3, $4, *$5, $6); }
|	identifier buffer_descriptor identifier variable_attribute { $$ = new VariableDeclaration(VariableDeclaration::Qualifier::None, *$1, $2, ReservedToken::None, *$3, $4); }
|	qualifier identifier buffer_descriptor identifier variable_attribute { $$ = new VariableDeclaration($1, *$2, $3, ReservedToken::None, *$4, $5); }
|	identifier buffer_descriptor reserved_token identifier variable_attribute { $$ = new VariableDeclaration(VariableDeclaration::Qualifier::None, *$1, $2, $3, *$4, $5); }		
		;

variable_list:  variable_list COMMA variable_declaration { $$->_variableDeclarations.push_back($3);}
	|	variable_declaration { $$ = new VariableList() ;  $$->_variableDeclarations.push_back($1); }
		;

function_declaration : identifier identifier identifier BEGIN_BRACKET variable_list END_BRACKET BEGIN_CURLY_BRACKET statements END_CURLY_BRACKET { $$ = new FunctionDeclaration(*$1, *$2, *$3, $5, $8); }
		;

statement:  USING_NAMESPACE identifier SEMICOLON {  $$ = new UsingDeclaration(*$2); }
	| 	struct SEMICOLON { $$ = $1; }
	|	function_declaration { $$ = $1; }
	| 	variable_declaration SEMICOLON { $$ = $1; }
	|	variable_declaration ASSIGN expression SEMICOLON { $$ = nullptr; }
	|	expression ASSIGN expression SEMICOLON { $$ = nullptr; }
	|	RETURN expression SEMICOLON { $$ = nullptr; }
	|	expression SEMICOLON { $$ = $1; }
		;

identifier: IDENTIFIER { $$ = new std::string(*$1); delete $1; }
		;

function_argument_list:
		function_argument_list COMMA expression {}
	|	expression { }
	;

function_call: 	identifier BEGIN_BRACKET function_argument_list END_BRACKET {}
	|  	identifier BEGIN_BRACKET END_BRACKET {}	
	;

constant:	INT_VALUE { $$ = new ConstantExpression(ConstantType::Int, *$1); }
	|	HALF_VALUE { $$ = new ConstantExpression(ConstantType::Half, *$1); }
	|	FLOAT_VALUE { $$ = new ConstantExpression(ConstantType::Float, *$1); }
	|	DOUBLE_VALUE { $$ = new ConstantExpression(ConstantType::Double, *$1); }
		;

expression1: constant
	| 	identifier	{ $$ = new ConstantExpression(ConstantType::Identifier, *$1); }
	| 	BEGIN_BRACKET expression END_BRACKET { $$ = new UnaryExpression(UnaryType::Parenthesis, $1); }
	| 	function_call
	| 	MINUS expression1 { $$ = new UnaryExpression(UnaryType::Minus, $1); }
;


expression0:
		expression0 STAR expression1 {  $$ = new BinaryExpression($1, BinaryOperator::Plus, $2); }
	|	expression0 FORWARD_SLASH expression1 {  $$ = new BinaryExpression($1, BinaryOperator::Plus, $2); }
	| 	expression1 { $$ = $1; }
	| 	expression0 DOT expression1 {  $$ = new BinaryExpression($1, BinaryOperator::Plus, $2); }
		;

expression:
	       expression PLUS expression0 {  $$ = new BinaryExpression($1, BinaryOperator::Plus, $2); }
	| expression MINUS expression0 {  $$ = new BinaryExpression($1, BinaryOperator::Minus, $2); }
	| expression0 { $$ = $1; }
	;

%%
		
