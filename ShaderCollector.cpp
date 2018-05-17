#include "ShaderCollector.h"

std::vector<struct FunctionDeclaration *> ShaderCollector::collect(Block * root)
{
  _shaders.clear();
  
  if(root!=nullptr)
    root->visit(this);

  return _shaders;
  
}


void ShaderCollector::operateOn(struct AssignStatement * desc)
{
  desc->_left->visit(this);
  desc->_right->visit(this);
}

void ShaderCollector::operateOn(struct BinaryExpression * desc)
{
  desc->_left->visit(this);
  desc->_right->visit(this);
}

void ShaderCollector::operateOn(struct Block * block)
{
  for(auto node : block->_nodes)
    node->visit(this);
}

void ShaderCollector::operateOn(struct BufferDescriptor * desc)
{
  // leaf node
}

void ShaderCollector::operateOn(struct ConstantExpression * desc)
{
  // leaf node  
}

void ShaderCollector::operateOn(struct Expression * desc)
{
  // leaf node
}

void ShaderCollector::operateOn(struct FunctionCall * node)
{
  if(node->_arguments!=nullptr)
    node->_arguments->visit(this);
}

void ShaderCollector::operateOn(struct FunctionCallArgumentList * node)
{
  for(auto arg : node->_expressions)
    arg->visit(this);
}

void ShaderCollector::operateOn(struct FunctionDeclaration * node)
{
  // leaf node
  switch(node->_functionType)
    {
    case FunctionType::Vertex:
    case FunctionType::Fragment:
      _shaders.push_back(node);
      break;
    default:
      break;
    }
}

void ShaderCollector::operateOn(struct Node * node)
{
  // leaf node
}

void ShaderCollector::operateOn(struct Program * program)
{
  for(auto node : program->_nodes)
    node->visit(this);
}

void ShaderCollector::operateOn(struct ReturnStatement * statement)
{
  if(statement->_expression != nullptr)
    statement->_expression->visit(this);
}

void ShaderCollector::operateOn(struct Statement * statement)
{
  // leaf node
}

void ShaderCollector::operateOn(struct Struct * strct)
{
  strct->_block.visit(this);
}

void ShaderCollector::operateOn(struct UnaryExpression * desc)
{
  desc->_expression->visit(this);
}

void ShaderCollector::operateOn(struct UsingDeclaration * usingDecl)
{
  // leaf node
}

void ShaderCollector::operateOn(struct VariableAttribute * node)
{
  // leaf node
}

void ShaderCollector::operateOn(struct VariableDeclaration * node)
{
  // leaf node
}

void ShaderCollector::operateOn(struct VariableList * node)
{
  // leaf node
}
