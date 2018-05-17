#include <fstream>

#include "Ast2GL.h"
#include "ShaderCollector.h"
#include "Transpiler.h"

namespace
{
  std::string simplifiedFunctionSignature(FunctionDeclaration * fDecl)
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
    return output;
  }
}

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
    const std::string output = simplifiedFunctionSignature(fDecl);
    printf("\t%s\n", output.c_str());
  }
  printf("\n");

  // convert shaders
  printf("Converting and Writing Shaders\n");
  for(FunctionDeclaration * fDecl : _shaders)
  {
    const std::string simpleSignature = simplifiedFunctionSignature(fDecl);
    printf("\tConverting shader: %s\n", simpleSignature.c_str() );

    Transpiler transpiler;
    const std::string shader = transpiler.convert(block, fDecl);

    std::string extension = "";
    switch(fDecl->_functionType)
      {
      case FunctionType::Vertex:
	extension = "vert";
	break;
      case FunctionType::Fragment:
	extension = "frag";
	break;
      default:
	extension = "unknown";
	break;	
      }

    std::string filename = fDecl->_name + "." + extension;
    
    printf("\tWriting shader: %s\n",filename.c_str() );
    std::ofstream stream(filename, std::ofstream::out | std::ofstream::trunc );
    stream << shader;
    stream.close();
    
  }
  
  
}

