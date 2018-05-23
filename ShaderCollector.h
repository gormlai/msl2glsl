#ifndef _SHADER_COLLECTOR_H_
#define _SHADER_COLLECTOR_H_

#include "Types.h"

#include <vector>

class ShaderCollector
{
 public:
  static std::vector<struct FunctionDeclaration *>  collect(Block * root);
};


#endif
