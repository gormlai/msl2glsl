#include <stdio.h>
#include <iostream>

int yyerror(char const * err)
{
  
  std::cerr << "Cannot parse input: " <<  err;
  return 0;
}

int main(int argc, char ** argv)
{
  return 0;
  
}
