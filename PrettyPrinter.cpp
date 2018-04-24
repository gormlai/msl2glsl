#include "PrettyPrinter.h"
#include "Types.h"

void PrettyPrinter::indent()
{
  for(int i =0 ; i < _indent ; i++)
    _result = _result + "  ";
}

const std::string PrettyPrinter::print(struct Block * block)
{
  
  _result = std::string("\n");
  _indent = 0;
  
  if(block == nullptr) {
    printf("PrettyPrinter - program is nullptr\n");
    return _result;
  }

  
  block->visit(this);
  
  return _result;
}

void PrettyPrinter::operateOn(struct Struct * strct)
{
  indent();
  _result = _result + "struct " + strct->_name + "\n";

  strct->_block.visit(this);
  
  indent();
  _result = _result + ";\n";
  
}

void PrettyPrinter::operateOn(struct UsingDeclaration * usingDecl)
{
  indent();
  _result = _result + "using namespace " + usingDecl->_nmspace + ";\n";
}

void PrettyPrinter::operateOn(struct Block * block)
{
  indent();
  _result = _result + "{\n";

  _indent++;

  for(auto node : block->_nodes)
    node->visit(this);

  _indent--;
  
  indent();
  _result = _result + "}";
}

void PrettyPrinter::operateOn(struct Program * program)
{
  for(auto node : program->_nodes)
    node->visit(this);
}


void PrettyPrinter::operateOn(struct Declaration * decl)
{
}

void PrettyPrinter::operateOn(struct Node * node)
{
}

void PrettyPrinter::operateOn(struct UChar * node)
{
  _result = _result + "uchar";
}

void PrettyPrinter::operateOn(struct UChar2 * node)
{
  _result = _result + "uchar2";
}

void PrettyPrinter::operateOn(struct UChar3 * node)
{
  _result = _result + "uchar3";
}

void PrettyPrinter::operateOn(struct UChar4 * node)
{
  _result = _result + "uchar4";
}

void PrettyPrinter::operateOn(struct Float * node)
{
  _result = _result + "float";
}

void PrettyPrinter::operateOn(struct Float2 * node)
{
  _result = _result + "float2";
}

void PrettyPrinter::operateOn(struct Float3 * node)
{
  _result = _result + "float3";
}

void PrettyPrinter::operateOn(struct Float4 * node)
{
  _result = _result + "float4";
}

void PrettyPrinter::operateOn(struct VariableDeclaration * node)
{
  indent();
  node->_type->visit(this);
  _result = _result + " " + node->_variableName + ";\n";
  
}
