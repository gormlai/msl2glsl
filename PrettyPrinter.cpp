#include "PrettyPrinter.h"
#include "Types.h"

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

void PrettyPrinter::operateOn(struct Program * program)
{
}

void PrettyPrinter::operateOn(struct Struct * strct)
{
  printf("PrettyPrinter::operateOnStruct\n");
}

void PrettyPrinter::operateOn(struct UsingDeclaration * usingDecl)
{
  _result = _result + "using namespace " + usingDecl->_nmspace + ";\n";
}

void PrettyPrinter::operateOn(struct DeclarationList * declList)
{
}

void PrettyPrinter::operateOn(struct Declaration * decl)
{
  printf("PrettyPrinter::operateOnDeclaration\n");
}

void PrettyPrinter::operateOn(struct Node * node)
{
  printf("PrettyPrinter::operateOnNode\n");
}

