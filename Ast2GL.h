#ifndef _AST_TO_GL_H_
#define _AST_TO_GL_H_

#include "Types.h"
#include <vector>

class Ast2GL
{
 public:
  void convert(Block * root);

 private:
  std::vector<struct FunctionDeclaration *> _shaders;

  
};

#endif
