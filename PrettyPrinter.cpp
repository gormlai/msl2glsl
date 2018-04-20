#include "PrettyPrinter.h"
#include "Types.h"

void PrettyPrinter::indent()
{
  for(int i =0 ; i < _indent ; i++)
    printf("\t");
}

const std::string PrettyPrinter::print(struct Program * program)
{
  
  _result = std::string("");
  
  if(program == nullptr) {
    printf("PrettyPrinter - program is nullptr\n");
    return _result;
  }

  program->visit(this);
  
  return _result;
}

void PrettyPrinter::preOperateOn(struct Program * program)
{
  indent();
}

void PrettyPrinter::preOperateOn(struct Struct * strct)
{
  indent();
  printf("PrettyPrinter::operateOnStruct\n");
}

void PrettyPrinter::preOperateOn(struct UsingDeclaration * usingDecl)
{
  indent();
  _result = _result + "using namespace " + usingDecl->_nmspace + ";\n";
}

void PrettyPrinter::preOperateOn(struct DeclarationList * declList)
{
  indent();
}

void PrettyPrinter::preOperateOn(struct Declaration * decl)
{
  indent();
  printf("PrettyPrinter::operateOnDeclaration\n");
}

void PrettyPrinter::preOperateOn(struct Node * node)
{
  indent();
  printf("PrettyPrinter::operateOnNode\n");
}

void PrettyPrinter::postOperateOn(struct Program * program)
{
  indent();
}

void PrettyPrinter::postOperateOn(struct Struct * strct)
{
  indent();
}

void PrettyPrinter::postOperateOn(struct UsingDeclaration * usingDecl)
{
  indent();
}

void PrettyPrinter::postOperateOn(struct DeclarationList * declList)
{
  indent();
}

void PrettyPrinter::postOperateOn(struct Declaration * decl)
{
  indent();
}

void PrettyPrinter::postOperateOn(struct Node * node)
{
  indent();
}
