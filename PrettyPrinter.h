#ifndef _PRETTY_PRINTER_H_
#define _PRETTY_PRINTER_H_

#include "Visitor.h"
#include <string>

class PrettyPrinter : public Visitor
{
 public:
  virtual ~PrettyPrinter() {}
  
  const std::string  print(struct Block * block);

  void preOperateOn(struct Struct * strct) override;
  void preOperateOn(struct UsingDeclaration * usingDecl) override;
  void preOperateOn(struct Block * block) override;
  void preOperateOn(struct Declaration * decl) override;
  void preOperateOn(struct Node * node) override;  
  
  void postOperateOn(struct Struct * strct) override;
  void postOperateOn(struct UsingDeclaration * usingDecl) override;
  void postOperateOn(struct Block * block) override;
  void postOperateOn(struct Declaration * decl) override;
  void postOperateOn(struct Node * node) override;

 private:
  void indent();
  
  std::string _result;
  int _indent;
  
};

#endif
