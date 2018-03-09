%{

  int yylex();
  int yyerror(char const *s);
%}

%token END               0 "end of file"
%token EOL                 "end of line"
%token BOOL		      
%token FLOAT
%token HALF		      
%token DOUBLE		      
%token INT


%%
input:
  BOOL { $$ = $1;}
;
