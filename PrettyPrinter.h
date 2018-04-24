#ifndef _PRETTY_PRINTER_H_
#define _PRETTY_PRINTER_H_

#include "Visitor.h"
#include <string>

class PrettyPrinter : public Visitor
{
 public:
  virtual ~PrettyPrinter() {}
  
  const std::string  print(struct Block * block);

  void operateOn(struct Struct * strct) override;
  void operateOn(struct UsingDeclaration * usingDecl) override;
  void operateOn(struct Block * block) override;
  void operateOn(struct Program * program) override;
  void operateOn(struct Declaration * decl) override;
  void operateOn(struct Node * node) override;  
  void operateOn(struct UChar * node) override;
  void operateOn(struct UChar2 * node) override;
  void operateOn(struct UChar3 * node) override;
  void operateOn(struct UChar4 * node) override;
  void operateOn(struct Float * node) override;
  void operateOn(struct Float2 * node) override;
  void operateOn(struct Float3 * node) override;
  void operateOn(struct Float4 * node) override;
  void operateOn(struct VariableDeclaration * node) override;
  
 private:
  void indent();
  
  std::string _result;
  int _indent;
  
};

#endif
