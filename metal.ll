%{
#include <stdlib.h>
#include <iostream>
int yyerror(char const *s);
int yylex();

#include "metal.tab.h"
%}

%option noyywrap


			
%%
[\t]          ; 

("true"|"false")    {return BOOL;}
[0-9]+\.[0-9]+"f"  { return FLOAT; }
[0-9]+\.[0-9]+"h"  { return HALF; }
[0-9]+\.[0-9]+  { return DOUBLE; }
[0-9]+  { return INT; }
%% 

int main(int argc, char ** argv)
{
	if(argc<=1)
	{
		std::cerr << "Not enough arguments. Give path to metal file " << std::endl;
		return 1;
	}

	FILE * file = fopen(argv[1],"r");
	if(file == NULL)
	{
		std::cerr << "Cannot find file: " << argv[1] << std::endl;
		return 2;
	}

	yyin = file;
	yylex();
	return 0;
}
