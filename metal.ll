%{
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
%option nodefault
%option yyclass = "Metal::Parser"
%option noyywrap
%option c++


%%

%{
//  yylval = lval;
%}

[0-9]+ {
//  yylval->build(atoi(yytext));
  return token::INT;
}

[0-9]+"."[0-9]* {
 // yylval->build(atof(yytext));
  return token::DOUBLE;
}

[0-9]+"."[0-9]*"f" {
  //yylval->build(atof(yytext));
  return token::FLOAT;
}

[0-9]+"."[0-9]*"h" {
  //yylval->build(atof(yytext));
  return token::HALF;
}

\n {
  loc->lines();
  return token::EOL;  
}

%%

