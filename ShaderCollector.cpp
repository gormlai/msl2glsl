#include "ShaderCollector.h"

std::vector<struct FunctionDeclaration *> ShaderCollector::collect(Block * root)
{
  std::vector<struct FunctionDeclaration *> shaders;
  
  for(auto node : root->_nodes) {
    if(node->getNodeType() == NodeType::FunctionDeclaration) {
      FunctionDeclaration * function = (FunctionDeclaration*)node;
      switch(function->_functionType) {
      case FunctionType::Vertex:
      case FunctionType::Fragment:
	shaders.push_back(function);
	break;
      default:
	break;
      }
    }
  }
  return shaders;
  
}


