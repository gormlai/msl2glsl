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

#define yyterminate() return( token::END )
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option debug
%option yyclass = "Metal::Parser"
%option noyywrap
%option c++
%option verbose
%option yylineno
			
%%

%{
  _yyval = lval;
%}


"struct"                                   { return token::STRUCT;}
"{"                                        { return token::BEGIN_CURLY_BRACKET; }
"}"                                        { return token::END_CURLY_BRACKET; }

"using namespace"     { return token::USING_NAMESPACE; }
";"                                        { return token::SEMICOLON; }
"float" { return token::FLOAT; }
"float2" { return token::FLOAT2; }
"float3" { return token::FLOAT3; }
"float4" { return token::FLOAT4; }

"uchar" { return token::UCHAR; }
"uchar2" { return token::UCHAR2; }
"uchar3" { return token::UCHAR3; }
"uchar4" { return token::UCHAR4; }

"(" { return token::BEGIN_BRACKET; }
")" { return token::END_BRACKET; }

"[[" { return token::BEGIN_DOUBLE_SQUARE_BRACKET; }
"]]" { return token::END_DOUBLE_SQUARE_BRACKET; }
{LETTER}{LETTER_OR_DIGIT}*                 { _yyval->build<std::string>(yytext); return token::IDENTIFIER; }
{WHITESPACE}                              { /* skip */ }

. { std::cerr << "Error line:" << yylineno << "\t" << yytext << std::endl; }

%%
