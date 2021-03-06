#ifndef _PRETTY_PRINTER_H_
#define _PRETTY_PRINTER_H_

#include "Visitor.h"
#include <string>
#include <vector>

class PrettyPrinter : public Visitor
{
 public:
  virtual ~PrettyPrinter() {}
  
  const std::string  print(struct Block * block);

  void operateOn(struct Assignment * desc) override;  
  void operateOn(struct BinaryExpression * desc) override;  
  void operateOn(struct Block * block) override;
  void operateOn(struct BufferDescriptor * desc) override;
  void operateOn(struct CastExpression * desc) override;
  void operateOn(struct CompareExpression * desc) override;
  void operateOn(struct ConstantExpression * desc) override;
  void operateOn(struct DeclarationSpecifier * desc) override;
  void operateOn(struct DeclarationSpecifierList * desc) override;
  void operateOn(struct Expression * desc) override;  
  void operateOn(struct ForLoop * node) override;
  void operateOn(struct FunctionCall * node) override;
  void operateOn(struct FunctionCallArgumentList * node) override;
  void operateOn(struct FunctionDeclaration * node) override;
  void operateOn(struct JumpStatement * statement) override;
  void operateOn(struct LabeledStatement * node) override;
  void operateOn(struct Preprocessor * def) override;  
  void operateOn(struct Program * program) override;
  void operateOn(struct Qualifier * qualifier) override;
  void operateOn(struct SelectExpression * exp) override;
  void operateOn(struct SelectionStatement * statement) override;
  void operateOn(struct Statement * statement) override;
  void operateOn(struct Struct * strct) override;
  void operateOn(struct TypeSpecifier * node) override;
  void operateOn(struct UnaryExpression * desc) override;
  void operateOn(struct UsingDeclaration * usingDecl) override;
  void operateOn(struct VariableAttribute * node) override;
  void operateOn(struct VariableDeclaration * node) override;
  void operateOn(struct VariableNameDeclaration * node) override;
  void operateOn(struct VariableList * node) override;
  
 private:
  void indent();
  void arrayNumToString(Expression * e);
  void toCommaSeparatedList(const std::vector<struct VariableNameDeclaration *> & input);
  
  
  std::string _result;
  int _indent;
  
};

#endif
