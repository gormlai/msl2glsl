#include "PrettyPrinter.h"
#include "Types.h"

void PrettyPrinter::indent()
{
  for(int i =0 ; i < _indent ; i++)
    printf("\t");
}

const std::string PrettyPrinter::print(struct Block * block)
{
  
  _result = std::string("");
  _indent = 0;
  
  if(block == nullptr) {
    printf("PrettyPrinter - program is nullptr\n");
    return _result;
  }

  block->visit(this);
  
  return _result;
}


void PrettyPrinter::preOperateOn(struct Struct * strct)
{
  //indent();
  _result = _result + "struct " + strct->_name + "\n";
  
  //indent();
  _result = _result + "{\n";

  _indent++;
  
}

void PrettyPrinter::preOperateOn(struct UsingDeclaration * usingDecl)
{
  indent();
  _result = _result + "using namespace " + usingDecl->_nmspace + ";\n";
}

void PrettyPrinter::preOperateOn(struct Block * block)
{
}

void PrettyPrinter::preOperateOn(struct Declaration * decl)
{
}

void PrettyPrinter::preOperateOn(struct Node * node)
{
}

void PrettyPrinter::postOperateOn(struct Struct * strct)
{
  //indent();
  _result = _result + "};\n";
  
  _indent--;
}

void PrettyPrinter::postOperateOn(struct UsingDeclaration * usingDecl)
{
}

void PrettyPrinter::postOperateOn(struct Block * block)
{
}

void PrettyPrinter::postOperateOn(struct Declaration * decl)
{
}

void PrettyPrinter::postOperateOn(struct Node * node)
{
}
