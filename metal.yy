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
    FunctionCall * functionCall;
    FunctionCallArgumentList * functionCallArgumentList;
    std::vector<VariableNameDeclaration* > * variableNameList;
    AssignOperator assignOperator;

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
%token                STATIC
%token                VERTEX
%token                FRAGMENT
%token 		      SKIP
%token 		      STRUCT
%token		      SEMICOLON
%token		      COLON
%token		      QUESTION_MARK
%token		      COMMA
%token                DOT
%token                POINTER
%token                ASSIGN_EQUAL
%token                ASSIGN_PLUS
%token                ASSIGN_MINUS
%token                ASSIGN_MULTIPLY
%token                ASSIGN_DIVIDE
%token                PLUS
%token                MINUS
%token                STAR
%token                AMPERSAND
%token		      FORWARD_SLASH
%token		      RETURN
%token	<qualifier>   CONSTANT
%token	<qualifier>   CONST
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
%token                BEGIN_SINGLE_SQUARE_BRACKET
%token                END_SINGLE_SQUARE_BRACKET
%token                BEGIN_DOUBLE_SQUARE_BRACKET
%token                END_DOUBLE_SQUARE_BRACKET
%token                DOUBLE_COLON
%token                ACCESS
%token                LESS_THAN
%token                GREATER_THAN
%token                LESS_THAN_OR_EQUAL
%token                GREATER_THAN_OR_EQUAL
%token                EQUAL_TO
%token                NOT_EQUAL_TO
%token                TYPEDEF
%token                IF
%token                ELSEIF
%token                ELSE
%token                FOR

%type	<block> compound_statement
%type	<block> statement_list
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
%type	<expression>   cast_expression
%type	<expression>   compare_expression
%type	<expression>   constant
%type	<expression>   expression_statement
%type	<functionCall>   function_call
%type	<functionCallArgumentList> function_argument_list
%type	<variableNameList> variable_name_list
%type	<intValue>     array_declaration
%type	<assignOperator> assign_operator
%locations

%%
translation_unit: statement_list { _root = new Program($1); $$ = _root; delete $1; }
		;

struct: STRUCT identifier compound_statement { $$ = new Struct(*$2); $$->_block = *$3; delete $3; }
	| TYPEDEF STRUCT compound_statement identifier { $$ = new Struct(*$4); $$->_block = *$3; delete $3; }
	| TYPEDEF STRUCT identifier compound_statement identifier { $$ = new Struct(*$5); $$->_block = *$4; delete $4; }
		;

statement_list:	
		statement_list statement { $$->_nodes.push_back($2); $2->_parent = $$; }
	|	statement { $$ = new Block() ;  $$->_nodes.push_back($1); $1->_parent = $$; }
	;

compound_statement: 	BEGIN_CURLY_BRACKET END_CURLY_BRACKET { $$ = new Block(); }
	|      BEGIN_CURLY_BRACKET statement_list END_CURLY_BRACKET { $$ = $2; }
		;

variable_attribute: /* empty */ { $$ = nullptr; }

	|	BEGIN_DOUBLE_SQUARE_BRACKET identifier BEGIN_BRACKET INT_VALUE END_BRACKET END_DOUBLE_SQUARE_BRACKET { $$ = new VariableAttribute(*$2,$4); }
	|	BEGIN_DOUBLE_SQUARE_BRACKET identifier END_DOUBLE_SQUARE_BRACKET { $$ = new VariableAttribute(*$2); }	
	;

buffer_descriptor:
		      LESS_THAN identifier COMMA ACCESS DOUBLE_COLON identifier GREATER_THAN { $$ = new BufferDescriptor(*$2, *$6); }
	;

qualifier:
		CONSTANT { $$ = VariableDeclaration::Qualifier::Constant; }
	|	CONST { $$ = VariableDeclaration::Qualifier::Const; }
	;

reserved_token:
	       AMPERSAND { $$ = ReservedToken::Ampersand; }
	|      STAR {$$ = ReservedToken::Star; }
	;

array_declaration:
		BEGIN_SINGLE_SQUARE_BRACKET INT_VALUE END_SINGLE_SQUARE_BRACKET { $$ = $2; }
	;

variable_name_list: /* variable_name_list COMMA identifier { $$->push_back( new VariableNameDeclaration(*$3, 0)); }*/
		identifier array_declaration { $$ = new std::vector<VariableNameDeclaration * >(); $$->push_back(new VariableNameDeclaration(*$1, $2)); }
|		identifier { $$ = new std::vector<VariableNameDeclaration * >(); $$->push_back(new VariableNameDeclaration(*$1, 0)); }
		;

variable_declaration:
	qualifier identifier reserved_token variable_name_list variable_attribute { $$ = new VariableDeclaration($1, *$2, nullptr, $3, *$4, $5); }
|	identifier variable_name_list variable_attribute { $$ = new VariableDeclaration(VariableDeclaration::Qualifier::None, *$1, nullptr, ReservedToken::None, *$2, $3); }
|	qualifier identifier variable_name_list variable_attribute { $$ = new VariableDeclaration($1, *$2, nullptr, ReservedToken::None, *$3, $4); }
|	identifier reserved_token variable_name_list variable_attribute { $$ = new VariableDeclaration(VariableDeclaration::Qualifier::None, *$1, nullptr, $2, *$3, $4); }
|	qualifier identifier buffer_descriptor reserved_token variable_name_list variable_attribute { $$ = new VariableDeclaration($1, *$2, $3, $4, *$5, $6); }
|	identifier buffer_descriptor variable_name_list variable_attribute { $$ = new VariableDeclaration(VariableDeclaration::Qualifier::None, *$1, $2, ReservedToken::None, *$3, $4); }
|	qualifier identifier buffer_descriptor variable_name_list variable_attribute { $$ = new VariableDeclaration($1, *$2, $3, ReservedToken::None, *$4, $5); }
|	identifier buffer_descriptor reserved_token variable_name_list variable_attribute { $$ = new VariableDeclaration(VariableDeclaration::Qualifier::None, *$1, $2, $3, *$4, $5); }
		;

variable_list:  variable_list COMMA variable_declaration { $$->_variableDeclarations.push_back($3); $3->_parent = $$; }
	|	variable_declaration { $$ = new VariableList() ;  $$->_variableDeclarations.push_back($1); $1->_parent = $$; }
		;

function_declaration : VERTEX identifier identifier BEGIN_BRACKET variable_list END_BRACKET compound_statement { $$ = new FunctionDeclaration(FunctionType::Vertex, *$2, *$3, $5, $7); }
	| FRAGMENT identifier identifier BEGIN_BRACKET variable_list END_BRACKET compound_statement { $$ = new FunctionDeclaration(FunctionType::Fragment, *$2, *$3, $5, $7); }
	| STATIC identifier identifier BEGIN_BRACKET variable_list END_BRACKET compound_statement { $$ = new FunctionDeclaration(FunctionType::Utility, *$2, *$3, $5, $7); }
		| identifier identifier BEGIN_BRACKET variable_list END_BRACKET compound_statement { $$ = new FunctionDeclaration(FunctionType::Utility, *$1, *$2, $4, $6); }
		;

assign_operator:
		ASSIGN_EQUAL { $$ = AssignOperator::Equal; }
	|	ASSIGN_PLUS { $$ = AssignOperator::EqualPlus; }
	|	ASSIGN_MINUS { $$ = AssignOperator::EqualMinus; }
	|	ASSIGN_MULTIPLY { $$ = AssignOperator::EqualMultiply; }
	|	ASSIGN_DIVIDE { $$ = AssignOperator::EqualDivide; }
	;

expression_statement:
		SEMICOLON { /* empty */ $$ = nullptr; }
	|	expression SEMICOLON { $$ = $1; }

	;

statement:  	USING_NAMESPACE identifier SEMICOLON {  $$ = new UsingDeclaration(*$2); }
	| 	struct SEMICOLON { $$ = $1; }
	|	function_declaration { $$ = $1; }
	| 	variable_declaration SEMICOLON { $$ = $1; }
	|	variable_declaration assign_operator expression SEMICOLON { $$ = new AssignStatement($1, $2, $3); }
	|	variable_declaration assign_operator BEGIN_CURLY_BRACKET function_argument_list END_CURLY_BRACKET SEMICOLON { $$ = new AssignStatement($1, $2, $4); }
	|	expression assign_operator expression SEMICOLON { $$ = new AssignStatement($1, $2, $3); }
	|	RETURN expression SEMICOLON { $$ = new ReturnStatement($2); }
	|	IF BEGIN_BRACKET expression END_BRACKET statement { $$ = new IfStatement(IfStatementType::If, $3, $5); }
	|	ELSEIF BEGIN_BRACKET expression END_BRACKET statement { $$ = new IfStatement(IfStatementType::ElseIf, $3, $5); }
	|	ELSE statement { $$ = new IfStatement(IfStatementType::Else, nullptr, $2); }
	|	FOR BEGIN_BRACKET expression_statement expression_statement END_BRACKET statement { $$ = new ForLoop($3, $4, nullptr, $6); }
	|	FOR BEGIN_BRACKET expression_statement expression_statement expression END_BRACKET statement { $$ = new ForLoop($3, $4, $5, $7); }
	|	compound_statement { $$ = $1; }
	|	expression_statement { $$ = $1; }
		;

identifier: IDENTIFIER { $$ = new std::string(*$1); delete $1; }
		;

function_argument_list:
		function_argument_list COMMA expression { $$->_expressions.push_back($3); $3->_parent = $$; }
	|	expression { $$ = new FunctionCallArgumentList(); $$->_expressions.push_back($1); $1->_parent  = $$; }
	;

function_call: 	identifier BEGIN_BRACKET function_argument_list END_BRACKET { $$ = new FunctionCall(*$1, $3);}
	|  	identifier BEGIN_BRACKET END_BRACKET { $$ = new FunctionCall(*$1, nullptr); }	
	;

constant:	INT_VALUE { $$ = new ConstantExpression(ConstantType::Int, $1); }
	|	HALF_VALUE { $$ = new ConstantExpression(ConstantType::Half, $1); }
	|	FLOAT_VALUE { $$ = new ConstantExpression(ConstantType::Float, $1); }
	|	DOUBLE_VALUE { $$ = new ConstantExpression(ConstantType::Double, $1); }
		;

expression1: constant
	| 	identifier	{ $$ = new ConstantExpression(ConstantType::Identifier, *$1); }
	| 	BEGIN_BRACKET expression END_BRACKET { $$ = new UnaryExpression(UnaryType::Parenthesis, $2); }
	| 	function_call { $$ = $1; }
	| 	MINUS expression1 { $$ = new UnaryExpression(UnaryType::Minus, $2); }
;

compare_expression:
		expression1 { $$ = $1; }
	|	compare_expression GREATER_THAN expression1 { $$ = new CompareExpression($1, CompareOperator::GreaterThan, $3); }		
	|	compare_expression GREATER_THAN_OR_EQUAL expression1 { $$ = new CompareExpression($1, CompareOperator::GreaterThanOrEqualTo, $3); }		
	|	compare_expression LESS_THAN expression1 { $$ = new CompareExpression($1, CompareOperator::LessThan, $3); }		
	|	compare_expression LESS_THAN_OR_EQUAL expression1 { $$ = new CompareExpression($1, CompareOperator::LessThanOrEqualTo, $3); }		
	|	compare_expression EQUAL_TO expression1 { $$ = new CompareExpression($1, CompareOperator::EqualTo, $3); }
	|	compare_expression NOT_EQUAL_TO expression1 { $$ = new CompareExpression($1, CompareOperator::NotEqualTo, $3); }
	;

cast_expression:
		compare_expression { $$ = $1; }
	| 	BEGIN_BRACKET identifier END_BRACKET compare_expression { $$ = new CastExpression(*$2, $4); }		
	;

expression0:
		expression0 STAR cast_expression {  $$ = new BinaryExpression($1, BinaryOperator::Multiply, $3); }
	|	expression0 FORWARD_SLASH cast_expression {  $$ = new BinaryExpression($1, BinaryOperator::Divide, $3); }
	| 	cast_expression { $$ = $1; }
	| 	expression0 DOT cast_expression {  $$ = new BinaryExpression($1, BinaryOperator::Dot, $3); }
	| 	expression0 POINTER cast_expression {  $$ = new BinaryExpression($1, BinaryOperator::Pointer, $3); }
		;

expression:
		expression PLUS expression0 {  $$ = new BinaryExpression($1, BinaryOperator::Plus, $3); }
	| 	expression MINUS expression0 {  $$ = new BinaryExpression($1, BinaryOperator::Minus, $3); }
	|	expression QUESTION_MARK expression0 COLON expression0 { $$ = new SelectExpression($1, $3, $5); }
	| 	expression0 { $$ = $1; }
	;

%%
		
