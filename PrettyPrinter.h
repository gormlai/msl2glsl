#ifndef _PRETTY_PRINTER_H_
#define _PRETTY_PRINTER_H_

#include "Visitor.h"
#include <string>

class PrettyPrinter : public Visitor
{
 public:
  virtual ~PrettyPrinter() {}
  
  const std::string  print(struct Program * program);
  
  void operateOn(struct Program * program) override;
  void operateOn(struct Struct * strct) override;
  void operateOn(struct UsingDeclaration * usingDecl) override;
  void operateOn(struct DeclarationList * declList) override;
  void operateOn(struct Declaration * decl) override;
  void operateOn(struct Node * node) override;

 private:
  std::string _result;
  
};

#endif
