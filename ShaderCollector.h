#ifndef _SHADER_COLLECTOR_H_
#define _SHADER_COLLECTOR_H_

#include "Visitor.h"
#include "Types.h"

#include <vector>

class ShaderCollector : public Visitor
{
 public:
  std::vector<struct FunctionDeclaration *>  collect(Block * root);
  virtual ~ShaderCollector() {}
  
  void operateOn(struct AssignStatement * desc) override; 
  void operateOn(struct BinaryExpression * desc) override;
  void operateOn(struct Block * block) override;
  void operateOn(struct BufferDescriptor * desc) override;
  void operateOn(struct ConstantExpression * desc) override;  
  void operateOn(struct Expression * desc) override;  
  void operateOn(struct FunctionCall * node) override;
  void operateOn(struct FunctionCallArgumentList * node) override;
  void operateOn(struct FunctionDeclaration * node) override;
  void operateOn(struct Node * node) override;  
  void operateOn(struct Program * program) override;
  void operateOn(struct ReturnStatement * statement) override;
  void operateOn(struct Statement * statement) override;
  void operateOn(struct Struct * strct) override;
  void operateOn(struct UnaryExpression * desc) override;  
  void operateOn(struct UsingDeclaration * usingDecl) override;
  void operateOn(struct VariableAttribute * node) override;
  void operateOn(struct VariableDeclaration * node) override;  
  void operateOn(struct VariableList * node) override;  

 private:
  std::vector<struct FunctionDeclaration *> _shaders;

  
};


#endif
