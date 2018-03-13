#ifndef _PRE_PROCESSOR_H_
#define _PRE_PROCESSOR_H_

#include <iostream>

namespace Metal
{

class PreProcessor
{
 public:
  PreProcessor();

  std::string convertFile(const std::string & inFileName);
  std::string convertContents(const std::string & inData);
  
};

}

#endif
