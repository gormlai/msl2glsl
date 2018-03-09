#include <stdio.h>
#include <iostream>

int yyerror(char const * err)
{
  
  std::cerr << "Cannot parse input: " <<  err;
  return 0;
}

int k()
{
  printf("Hello World build with meson\n");
  return 0;
}
