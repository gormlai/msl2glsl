%{
#include <string.h>
#include <Scanner.h>

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
  _yyval = lval;
%}

[0-9]+ {
  _yyval->build(atoi(yytext));
  return token::INT;
}

[0-9]+"."[0-9]* {
  _yyval->build(atof(yytext));
  return token::DOUBLE;
}

[0-9]+"."[0-9]*"f" {
  _yyval->build(atof(yytext));
  return token::FLOAT;
}

[0-9]+"."[0-9]*"h" {
  _yyval->build(atof(yytext));
  return token::HALF;
}

\n {
  loc->lines();
  return token::EOL;  
}

. {
  std::cerr << "Error = " << yytext << std::endl;
}

%%

