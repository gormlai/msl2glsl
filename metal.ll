WHITESPACE [ \t\v\f]
NEWLINE [\n]
LETTER [_a-zA-Z]
DIGIT [0-9]
LETTER_OR_DIGIT ({LETTER}|{DIGIT})
FORWARDSLASH [/]
LETTER_OR_DIGIT_OR_FORWARDSLASH ({LETTER}|{DIGIT}|{FORWARDSLASH})

%{

#include <iostream>
#include "Types.h"
#include <string.h>
#include "Scanner.h"

#undef YY_DECL
#define YY_DECL int Metal::Scanner::yylex(Metal::Parser::semantic_type * const lval, Metal::Parser::location_type *loc)

using token = Metal::Parser::token;
int lines = 1;

//#define yyterminate() printf("end!\n"); fflush(NULL); return(EOF )
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option debug
%option yyclass = "Metal::Parser"
%option c++
%option verbose
%option noyywrap			
%%

%{
  _yyval = lval;
%}


"struct"                                   { return token::STRUCT;}
"constant"                                 { return token::CONSTANT;}
"{"                                        { return token::BEGIN_CURLY_BRACKET; }
"}"                                        { return token::END_CURLY_BRACKET; }
"/"                                        { return token::FORWARD_SLASH; }
"<"                                        { return token::LESS_THAN; }
">"                                        { return token::GREATER_THAN; }
"="                                        { return token::ASSIGN; }
"using namespace"                          { return token::USING_NAMESPACE; }
";"                                        { return token::SEMICOLON; }
","                                        { return token::COMMA; }
"+"                                        { return token::PLUS; }
"-"                                        { return token::MINUS; }
"*"                                        { return token::STAR; }
"&"                                        { return token::AMPERSAND; }
"."                                        { return token::DOT; }
"::"                                       { return token::DOUBLE_COLON; }
"access"                                   { return token::ACCESS; }
"("                                        { return token::BEGIN_BRACKET; }
")"                                        { return token::END_BRACKET; }
"[["                                       { return token::BEGIN_DOUBLE_SQUARE_BRACKET; }
"]]"                                       { return token::END_DOUBLE_SQUARE_BRACKET; }
"return"                                   { return token::RETURN; }
"typedef"                                  { return token::TYPEDEF; }
{DIGIT}*"."{DIGIT}+"f"                     { std::string t(yytext,yyleng); _yyval->floatValue = atof(t.c_str()); return token::FLOAT_VALUE; }
{DIGIT}*"."{DIGIT}+"h"                     { std::string t(yytext,yyleng); _yyval->halfValue = atof(t.c_str()); return token::HALF_VALUE; }
{DIGIT}*"."{DIGIT}+                        { std::string t(yytext,yyleng); _yyval->doubleValue = (double)atof(t.c_str()); return token::DOUBLE_VALUE; }
{DIGIT}*                                   { std::string t(yytext,yyleng); _yyval->intValue = atoi(t.c_str()); return token::INT_VALUE; }
{LETTER}{LETTER_OR_DIGIT}*                 { _yyval->string = new std::string(yytext,yyleng) ; return token::IDENTIFIER; }
{WHITESPACE}                               { /* skip */ }
{NEWLINE}                                  { lines++; }
"#include"{WHITESPACE}"<"[^\n]*">" { /* skip */ }
"//"[^\n]*                                 {/* skip */ }
	       

%%

void Metal::Parser::error(const location_type &line, const std::string &err)
{
    std::cerr << "Error at line: " << lines << ": " << err << std::endl; 
}
