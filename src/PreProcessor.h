#ifndef _PRE_PROCESSOR_H_
#define _PRE_PROCESSOR_H_

#include <iostream>

namespace Metal
{

class PreProcessor
{
 public:
  PreProcessor();

  std::string convert(const std::string & inFileName);
  
};

}

#endif
