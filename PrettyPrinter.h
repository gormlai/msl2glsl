#ifndef _PRETTY_PRINTER_H_
#define _PRETTY_PRINTER_H_

#include "Visitor.h"

class PrettyPrinter
{
 public:
  void operateOn(struct Program * program) override;
  void operateOn(struct Struct * strct) override;
  void operateOn(struct UsingDeclaration * usingDecl) override;
  void operateOn(struct DeclarationList * declList) override;
  void operateOn(struct Declaration * decl) override;
  void operateOn(struct Node * node) override;
  
};

#endif
