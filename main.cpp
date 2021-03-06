#include <stdio.h>
#include <iostream>
#include "Driver.h"


int yyerror(char const * err)
{
  printf("hest\n");
  return 0;
}

int yyerror(char const * err, ...)
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
  else if(argc>3) {
    std::cerr << "Too many arguments to program: " << argv[0] << std::endl;
    return (int)ProgramError::TooManyArguments;
  }

  std::string filename(argv[1]);
  std::string prettyPrintedFile;
  if(argc>=3)
    prettyPrintedFile = std::string(argv[2]);
  
  Metal::Driver driver;
 
  std::cout << "Filename = " + filename << std::endl;
  driver.convert(filename, prettyPrintedFile );
  
  return (int)ProgramError::NoError;
  
}
