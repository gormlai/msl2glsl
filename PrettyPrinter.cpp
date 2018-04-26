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

void PrettyPrinter::operateOn(struct Block * block)
{
  indent();
  _result = _result + "{\n";

  _indent++;

  for(auto node : block->_nodes) {
      node->visit(this);
      _result = _result + ";\n";
  }

  _indent--;
  
  indent();
  _result = _result + "}";
}


void PrettyPrinter::operateOn(struct Declaration * decl)
{
}

void PrettyPrinter::operateOn(struct FunctionDeclaration * node)
{
  indent();

  if(!node->_functionType.empty())
    _result = _result + node->_functionType + " ";

  if(!node->_returnType.empty())
    _result = _result + node->_returnType + " ";
  
  _result = _result + node->_name + "(";

  if(node->_variables!=nullptr)
    node->_variables->visit(this);
  
  _result = _result + ")\n";

  if(node->_block!=nullptr)
    node->_block->visit(this);  
}

void PrettyPrinter::operateOn(struct Node * node)
{
}

void PrettyPrinter::operateOn(struct Program * program)
{
  for(auto node : program->_nodes)
    node->visit(this);
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
  _result = _result + "using namespace " + usingDecl->_nmspace;
}

void PrettyPrinter::operateOn(struct VariableAttribute * attribute)
{
  _result = _result + " [[";
  
  if(attribute->_sAttribute == std::string("attribute"))
    _result = _result + "attribute(" + std::to_string(attribute->_iAttribute) + ")";
  else if(attribute->_sAttribute == std::string("position"))
    _result = _result + "position";
  else if(attribute->_sAttribute == std::string("stage_in"))
    _result = _result + "stage_in";

  _result = _result + "]]";  
}

void PrettyPrinter::operateOn(struct VariableDeclaration * node)
{
  indent();
  _result =  _result + node->_type + " " + node->_variableName;

  if(node->_attribute != nullptr)
    node->_attribute->visit(this);

  
}

void PrettyPrinter::operateOn(struct VariableList * node)
{
  const int count =  (int)node->_variableDeclarations.size(); 
  for(int i=0 ; i < count ; i++) {
    node->_variableDeclarations[i]->visit(this);
    
    if(i!=count-1)
      _result = _result + ", ";
  }
}

