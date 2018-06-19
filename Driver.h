#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "Scanner.h"
#include "metal.tab.h"
#include <memory>

namespace Metal
{
  
class Driver
{
 public:
  Driver();

  void convert(const std::string & filename, const std::string & prettyPrintedCompareFile);

 private:
  std::shared_ptr<Scanner> _scanner;
  std::shared_ptr<Parser> _parser;
};
}

#endif
