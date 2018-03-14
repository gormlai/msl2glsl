WHITESPACE [ \t\v\n\f]
STRING [_a-zA-Z0-9]

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
%option yyclass = "Metal::Parser"
%option noyywrap
%option c++

%%

%{
  _yyval = lval;
%}


"struct"                                   { return token::STRUCT;}

"using namespace"{WHITESPACE}+{STRING}+  { /* skip using namespace */}
";"                                        { return token::SEMICOLON; }

. { std::cerr << yytext; }

%%
