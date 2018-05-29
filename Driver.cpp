#include "Driver.h"
#include <fstream>
#include <sstream>
#include "Ast2GL.h"
#include "PrettyPrinter.h"


Metal::Driver::Driver()
{
}

void Metal::Driver::convert(const std::string & filename)
{
  if(filename.empty())
    return;

  std::ifstream stream(filename.c_str());
  
  _scanner = std::shared_ptr<Scanner>(new Scanner(&stream));
  _parser = std::shared_ptr<Parser>(new Parser(*_scanner.get(), *this));

  int parseResult = -1;
  try {


    parseResult = _parser->parse();
    if(parseResult == 0 )
    {
      
      printf("\n---------BEGIN PRETTY PRINTING FILE----------\n");
      PrettyPrinter printer;      
      const std::string result = printer.print(_root);
      printf("%s\n",result.c_str());
      printf("\n---------END PRETTY PRINTING FILE----------\n");
      
      if(_root != nullptr)
      {
		  printf("\n--------- BEGIN TRANSFORMING FILE ----------\n");
		  Ast2GL transpiler;
		  transpiler.convert(_root);
		  printf("\n--------- END TRANSFORMING FILE ----------\n");

      }
      
      
    }

  }
  catch(const std::bad_alloc & e) {
    std::cerr << "Bad Alloc: " << e.what() << std::endl;
  }
  catch(const std::exception & e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  catch(...) {
    std::cerr << "Unknown exception thrown while parsing file" << std::endl;
  }
  
  // make sure the parser is not holding any references to the scanner anymore
  _parser = std::shared_ptr<Parser>();

  // remove the scanner so it is not pointing to the stream anymore, if it is
  _scanner = std::shared_ptr<Scanner>();

  std::cout << "ParseResult = " << parseResult << std::endl;
  
}
