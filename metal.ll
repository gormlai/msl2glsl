WHITESPACE [ \t\v\n\f]
LETTER [_a-zA-Z]
DIGIT [0-9]
LETTER_OR_DIGIT ({LETTER}|{DIGIT})

%{

#include "Types.h"
#include <string.h>
#include <Scanner.h>

#undef YY_DECL
#define YY_DECL int Metal::Scanner::yylex(Metal::Parser::semantic_type * const lval, Metal::Parser::location_type *loc)

using token = Metal::Parser::token;

//#define yyterminate() printf("end!\n"); fflush(NULL); return(EOF )
#define YY_NO_UNISTD_H
//#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option debug
%option yyclass = "Metal::Parser"
%option c++
%option verbose
%option yylineno
%option nodefault
			
%%

%{
  _yyval = lval;
%}


"struct"                                   { printf("0\n"); return token::STRUCT;}
"{"                                        { printf("1\n");return token::BEGIN_CURLY_BRACKET; }
"}"                                        { printf("2\n");return token::END_CURLY_BRACKET; }

"using namespace"     { printf("3\n");return token::USING_NAMESPACE; }
";"                                        { printf("4\n");return token::SEMICOLON; }
"float" { printf("0\n");return token::TYPE_FLOAT; }
"float2" { printf("0\n");return token::TYPE_FLOAT2; }
"float3" { return token::TYPE_FLOAT3; }
"float4" { return token::TYPE_FLOAT4; }

"uchar" { return token::TYPE_UCHAR; }
"uchar2" { return token::TYPE_UCHAR2; }
"uchar3" { return token::TYPE_UCHAR3; }
"uchar4" { return token::TYPE_UCHAR4; }

"(" { return token::BEGIN_BRACKET; }
")" { return token::END_BRACKET; }

"[[" { return token::BEGIN_DOUBLE_SQUARE_BRACKET; }
"]]" { return token::END_DOUBLE_SQUARE_BRACKET; }
{LETTER}{LETTER_OR_DIGIT}*                 {printf("5\n"); _yyval->string = new std::string(yytext,yyleng) ; return token::IDENTIFIER; }
{WHITESPACE}                              {printf("6\n"); /* skip */ }

<<EOF>> {printf("7\n"); yyterminate(); return 0; }

. { std::cerr << "Error line:" << yylineno << "\t" << yytext << std::endl; }

%%

int yyFlexLexer::yywrap() { return 1; }
