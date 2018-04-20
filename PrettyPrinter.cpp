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
}

void PrettyPrinter::operateOn(struct UsingDeclaration * usingDecl)
{
}

void PrettyPrinter::operateOn(struct DeclarationList * declList)
{
}

void PrettyPrinter::operateOn(struct Declaration * decl)
{
}

void PrettyPrinter::operateOn(struct Node * node)
{
}

