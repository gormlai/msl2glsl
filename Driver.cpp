#include "Driver.h"
#include <fstream>

Metal::Driver::Driver()
{
}

void Metal::Driver::convert(const std::string & filename)
{
  if(filename.empty())
    return;
  
  std::ifstream stream(filename);
  
  _scanner = std::shared_ptr<Scanner>(new Scanner(&stream));
  _parser = std::shared_ptr<Parser>(new Parser(*_scanner.get(), *this));
  const int parseResult = _parser->parse();
  
  // make sure the parser is not holding any references to the scanner anymore
  _parser = std::shared_ptr<Parser>();

  // remove the scanner so it is not pointing to the stream anymore, if it is
  _scanner = std::shared_ptr<Scanner>();
  
}
