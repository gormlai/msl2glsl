#include "Ast2GL.h"
#include "ShaderCollector.h"

void Ast2GL::convert(Block * block)
{
  if(block == nullptr) {
    printf("Ast2Gl::convert -> program pointer is null");
  }

  /// collecting shaders to be converted
  ShaderCollector sCollector;
  printf("Collecting Shaders ... \n");
  _shaders = sCollector.collect(block);
  for(FunctionDeclaration * fDecl : _shaders)
  {
    std::string output = fDecl->_returnType + " " + fDecl->_name + "(...)";
    switch(fDecl->_functionType)
    {
    case FunctionType::Vertex:
      output = "vertex " + output;
      break;
    case FunctionType::Fragment:
      output = "fragment " + output;
      break;
    default:
      break;
    }

    printf("\t%s\n", output.c_str());
  }
  
  
}

