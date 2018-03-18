#include "Driver.h"
#include <fstream>
#include <sstream>
#include "PreProcessor.h"


Metal::Driver::Driver()
{
}

void Metal::Driver::convert(const std::string & filename)
{
  if(filename.empty())
    return;

  PreProcessor preProcessor;
  
  printf("PREPROCESSING FILE: %s\n", filename.c_str());
  std::string processedFileContents = preProcessor.convertFile(filename);
  printf("\n---------BEGIN PREPROCESSEDFILE----------\n%s\n",processedFileContents.c_str());

  // save out preprocessed contents
  std::string outFileName("preprocessed.metal");
  std::ofstream outStream(outFileName.c_str(), std::ofstream::out);
  outStream << processedFileContents;
  outStream.close();
  printf("\n---------END PREPROCESSEDFILE----------\n%s\n",processedFileContents.c_str());
  
  std::istringstream stream(processedFileContents);
//  std::ifstream stream(outFileName.c_str());
  
  _scanner = std::shared_ptr<Scanner>(new Scanner(&stream));
  _parser = std::shared_ptr<Parser>(new Parser(*_scanner.get(), *this));

  int parseResult = -1;
  try {
    std::cout << "t0" << std::endl;
    parseResult = _parser->parse();
    std::cout << "t1" << std::endl;
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
