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
    BinaryOperator binaryOperator;
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
%token		      TYPE_HEX	
%token		      TYPE_STRING
%token                STATIC
%token                VERTEX
%token                FRAGMENT
%token                KERNEL
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
%token                ASSIGN_MODULO
%token                ASSIGN_DIVIDE
%token                ASSIGN_XOR
%token                ASSIGN_OR
%token                ASSIGN_AND
%token                PLUS
%token                MINUS
%token                STAR
%token                PERCENT
%token                AMPERSAND
%token                PIPE
%token                HAT
%token                SWITCH
%token                CASE
%token                BREAK
%token                CONTINUE
%token                DOUBLE_AMPERSAND
%token                DOUBLE_PIPE
%token                LEFT_SHIFT
%token                RIGHT_SHIFT			
%token		      FORWARD_SLASH
%token		      RETURN
%token		      DEFAULT
%token		      PLUS_PLUS
%token		      MINUS_MINUS
%token	<qualifier>   CONSTANT
%token	<qualifier>   CONST
%token	<qualifier>   CONSTEXPR
%token	<string>      HEX_VALUE
%token	<string>      IDENTIFIER
%token	<string>      DEFINE
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
%type	<statement>	 jump_statement
%type	<statement>	 iteration_statement
%type	<statement>	 selection_statement
%type	<statement>	 init_statement
%type	<statement>	 simple_initialisation
%type	<statement>	 define
%type	<statement>	 labeled_statement
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
%type	<expression>   array_declaration
%type	<expression>   expression
%type	<expression>   expression1
%type	<expression>   binary_expression
%type	<expression>   assignment_expression
%type	<expression>   conditional_expression
%type	<expression>   cast_expression
%type	<expression>   compare_expression
%type	<expression>   constant
%type	<expression>   pre_postfix_expression
%type	<expression>   expression_statement
%type	<functionCall>   function_call
%type	<functionCallArgumentList> function_argument_list
%type	<variableNameList> variable_name_list
%type	<assignOperator> assign_operator
%type	<binaryOperator> binary_operator
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

	|	BEGIN_DOUBLE_SQUARE_BRACKET identifier BEGIN_BRACKET constant END_BRACKET END_DOUBLE_SQUARE_BRACKET { $$ = new VariableAttribute(*$2,$4); }
	|	BEGIN_DOUBLE_SQUARE_BRACKET identifier END_DOUBLE_SQUARE_BRACKET { $$ = new VariableAttribute(*$2); }	
	;

buffer_descriptor:
		      LESS_THAN identifier COMMA ACCESS DOUBLE_COLON identifier GREATER_THAN { $$ = new BufferDescriptor(*$2, *$6); }
	;

qualifier:
		CONSTANT { $$ = VariableDeclaration::Qualifier::Constant; }
	|	CONST { $$ = VariableDeclaration::Qualifier::Const; }
	|	CONSTEXPR { $$ = VariableDeclaration::Qualifier::Constexpr; }
	;

reserved_token:
	       AMPERSAND { $$ = ReservedToken::Ampersand; }
	|      STAR {$$ = ReservedToken::Star; }
	;

array_declaration:
		BEGIN_SINGLE_SQUARE_BRACKET expression END_SINGLE_SQUARE_BRACKET { $$ = $2; }
|	BEGIN_SINGLE_SQUARE_BRACKET END_SINGLE_SQUARE_BRACKET { $$ = new ConstantExpression(ConstantType::Int, -1); }
	;

variable_name_list: /* variable_name_list COMMA identifier { $$->push_back( new VariableNameDeclaration(*$3, 0)); }*/
		identifier array_declaration { $$ = new std::vector<VariableNameDeclaration * >(); $$->push_back(new VariableNameDeclaration(*$1, $2)); }
|		identifier { $$ = new std::vector<VariableNameDeclaration * >(); $$->push_back(new VariableNameDeclaration(*$1, nullptr)); }
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
	| KERNEL identifier identifier BEGIN_BRACKET variable_list END_BRACKET compound_statement { $$ = new FunctionDeclaration(FunctionType::Kernel, *$2, *$3, $5, $7); }
		| identifier identifier BEGIN_BRACKET variable_list END_BRACKET compound_statement { $$ = new FunctionDeclaration(FunctionType::Utility, *$1, *$2, $4, $6); }
		;

assign_operator:
		ASSIGN_EQUAL { $$ = AssignOperator::Equal; }
	|	ASSIGN_PLUS { $$ = AssignOperator::Plus; }
	|	ASSIGN_MINUS { $$ = AssignOperator::Minus; }
	|	ASSIGN_MULTIPLY { $$ = AssignOperator::Multiply; }
	|	ASSIGN_MODULO { $$ = AssignOperator::Modulo; }
	|	ASSIGN_DIVIDE { $$ = AssignOperator::Divide; }
	|	ASSIGN_XOR { $$ = AssignOperator::XOr; }
	|	ASSIGN_OR { $$ = AssignOperator::Or; }
	|	ASSIGN_AND { $$ = AssignOperator::And; }
	;

expression_statement:
		SEMICOLON { /* empty */ $$ = new Expression(); }
	|	expression SEMICOLON { $$ = $1; }

	;

selection_statement:
		IF BEGIN_BRACKET expression END_BRACKET statement { $$ = new SelectionStatement(SelectionStatementType::If, $3, $5); }
	|	ELSEIF BEGIN_BRACKET expression END_BRACKET statement { $$ = new SelectionStatement(SelectionStatementType::ElseIf, $3, $5); }
	|	ELSE statement { $$ = new SelectionStatement(SelectionStatementType::Else, nullptr, $2); }
	|	SWITCH BEGIN_BRACKET expression END_BRACKET statement { $$ = new SelectionStatement(SelectionStatementType::Switch, $3, $5); }
		
	;

simple_initialisation:
	 	variable_declaration { $$ = $1; }
	|	variable_declaration assign_operator expression { $$ = new Assignment($1, $2, $3); }
	|	variable_declaration assign_operator BEGIN_CURLY_BRACKET function_argument_list END_CURLY_BRACKET { $$ = new Assignment($1, $2, $4); }	
	;

init_statement:
		expression_statement { $$ = $1; }
	|	simple_initialisation SEMICOLON { $$ = $1; }
	;

iteration_statement:
		FOR BEGIN_BRACKET init_statement expression_statement END_BRACKET statement { $$ = new ForLoop($3, $4, nullptr, $6); }
 	|	FOR BEGIN_BRACKET init_statement expression_statement expression END_BRACKET statement { $$ = new ForLoop($3, $4, $5, $7); }
		
	;

jump_statement:
		RETURN SEMICOLON { $$ = new JumpStatement(JumpStatementType::Return, nullptr); }		
	|	RETURN expression SEMICOLON { $$ = new JumpStatement(JumpStatementType::Return, $2); }		
	|	CONTINUE SEMICOLON { $$ = new JumpStatement(JumpStatementType::Continue, nullptr); }
	|	BREAK SEMICOLON { $$ = new JumpStatement(JumpStatementType::Break, nullptr); }
		;

define:
		DEFINE { $$ = new Define(*$1); }
		;

labeled_statement:
		CASE conditional_expression COLON statement { $$ = new LabeledStatement(LabeledStatementType::Case, $2, $4); }
	|	DEFAULT COLON statement { $$ = new LabeledStatement(LabeledStatementType::Default, nullptr, $3); }
	;

statement:	
		compound_statement { $$ = $1; }
	|	init_statement { $$ = $1; }
	|	selection_statement { $$ = $1; }
	|	iteration_statement { $$ = $1; }
	|	jump_statement { $$ = $1; }
	|	labeled_statement { $$ = $1; }
	|	USING_NAMESPACE identifier SEMICOLON {  $$ = new UsingDeclaration(*$2); }
	|	define { $$ = $1; }
	| 	struct SEMICOLON { $$ = $1; }
	|	function_declaration { $$ = $1; }
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

constant:
		INT_VALUE { $$ = new ConstantExpression(ConstantType::Int, $1); }
	|	HEX_VALUE { $$ = new ConstantExpression(ConstantType::Hex, *$1); }
	|	HALF_VALUE { $$ = new ConstantExpression(ConstantType::Half, $1); }
	|	FLOAT_VALUE { $$ = new ConstantExpression(ConstantType::Float, $1); }
	|	DOUBLE_VALUE { $$ = new ConstantExpression(ConstantType::Double, $1); }
	| 	identifier	{ $$ = new ConstantExpression(ConstantType::Identifier, *$1); }
	| 	BEGIN_BRACKET expression END_BRACKET { $$ = new UnaryExpression(UnaryType::Parenthesis, $2); }
		;

pre_postfix_expression:
		constant
	|	MINUS_MINUS constant { $$ = new UnaryExpression(UnaryType::PreFixMinusMinus, $2); }
	|	PLUS_PLUS constant { $$ = new UnaryExpression(UnaryType::PreFixPlusPlus, $2); }
	|       constant PLUS_PLUS { $$ = new UnaryExpression(UnaryType::PostFixPlusPlus, $1); }
	|       constant MINUS_MINUS { $$ = new UnaryExpression(UnaryType::PostFixMinusMinus, $1); }
	| 	pre_postfix_expression BEGIN_SINGLE_SQUARE_BRACKET expression END_SINGLE_SQUARE_BRACKET { $$ = new BinaryExpression($1,BinaryOperator::Index, $3); }
	;

expression1:
		pre_postfix_expression        { $$ = $1; }
	| 	function_call { $$ = $1; }
	| 	MINUS expression1 { $$ = new UnaryExpression(UnaryType::Minus, $2); }
	| 	AMPERSAND expression1 { $$ = new UnaryExpression(UnaryType::TakeAddressOf, $2); }
	| 	STAR expression1 { $$ = new UnaryExpression(UnaryType::Dereference, $2); }
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

binary_operator:
		PIPE { $$ = BinaryOperator::BinaryOr; }
	|	DOUBLE_PIPE { $$ = BinaryOperator::LogicalOr; }
	|	AMPERSAND { $$ = BinaryOperator::BinaryAnd; }
	|	DOUBLE_AMPERSAND { $$ = BinaryOperator::LogicalAnd; }
	|	LEFT_SHIFT { $$ = BinaryOperator::LeftShift; }
	|	RIGHT_SHIFT { $$ = BinaryOperator::RightShift; }
	|	STAR { $$ = BinaryOperator::Multiply; }
	|	PERCENT { $$ = BinaryOperator::Modulo; }
	|	FORWARD_SLASH { $$ = BinaryOperator::Divide; }
	|	DOT { $$ = BinaryOperator::Dot; }
	|	POINTER { $$ = BinaryOperator::Pointer; }
	|	PLUS { $$ = BinaryOperator::Plus; }
	|	MINUS { $$ = BinaryOperator::Minus; }
	|	HAT { $$ = BinaryOperator::XOr; }
	;

binary_expression:
		cast_expression { $$ = $1; }
	|	binary_expression binary_operator cast_expression {  $$ = new BinaryExpression($1, $2, $3); }
		;

conditional_expression:
		binary_expression { $$ = $1; }
	|	binary_expression QUESTION_MARK expression COLON conditional_expression { $$ = new SelectExpression($1, $3, $5); }
	;

assignment_expression:
		conditional_expression { $$ = $1; }
	|	conditional_expression assign_operator assignment_expression { $$ = new Assignment($1, $2, $3); }
	;

expression:
	 	assignment_expression { $$ = $1; }
	;


%%
		
