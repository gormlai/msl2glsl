#ifndef _PRETTY_PRINTER_H_
#define _PRETTY_PRINTER_H_

#include "Visitor.h"
#include <string>

class PrettyPrinter : public Visitor
{
 public:
  virtual ~PrettyPrinter() {}
  
  const std::string  print(struct Block * block);

  void operateOn(struct Block * block) override;
  void operateOn(struct BufferDescriptor * desc) override;
  void operateOn(struct Declaration * decl) override;
  void operateOn(struct FunctionDeclaration * node) override;
  void operateOn(struct Node * node) override;  
  void operateOn(struct Program * program) override;
  void operateOn(struct Struct * strct) override;
  void operateOn(struct UsingDeclaration * usingDecl) override;
  void operateOn(struct VariableAttribute * node) override;
  void operateOn(struct VariableDeclaration * node) override;  
  void operateOn(struct VariableList * node) override;  
  
 private:
  void indent();
  
  std::string _result;
  int _indent;
  
};

#endif
