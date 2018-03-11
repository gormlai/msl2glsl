#include <stdio.h>
#include <iostream>

int yyerror(char const * err)
{
  
  std::cerr << "Cannot parse input: " <<  err;
  return 0;
}

enum class ProgramError
{
  NoError = 0,
    TooFewArguments,
    TooManyArguments,
};

int main(int argc, char ** argv)
{
  if(argc <2 ) {
    std::cerr << "Too few arguments to program: " << argv[0] << std::endl;
    return (int)ProgramError::TooFewArguments;
  }
  else if(argc>2) {
    std::cerr << "Too many arguments to program: " << argv[0] << std::endl;
    return (int)ProgramError::TooManyArguments;
  }

  
  
  return (int)ProgramError::NoError;
  
}
