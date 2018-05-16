#ifndef _VISITOR_H_
#define _VISITOR_H_

class Visitor
{
 public:
  virtual ~Visitor() {}
  
  virtual void operateOn(struct BinaryExpression * desc) {}  
  virtual void operateOn(struct Block * block) {}
  virtual void operateOn(struct BufferDescriptor * desc) {}
  virtual void operateOn(struct ConstantExpression * desc) {}  
  virtual void operateOn(struct Expression * desc) {}  
  virtual void operateOn(struct FunctionCall * node) {}
  virtual void operateOn(struct FunctionCallArgumentList * node) {}
  virtual void operateOn(struct FunctionDeclaration * node) {}
  virtual void operateOn(struct Node * node) {}
  virtual void operateOn(struct Program * program) {}
  virtual void operateOn(struct ReturnStatement * statement) {}
  virtual void operateOn(struct Statement * statement) {}
  virtual void operateOn(struct Struct * strct) {}
  virtual void operateOn(struct UnaryExpression * desc) {}  
  virtual void operateOn(struct UsingDeclaration * usingDecl) {}
  virtual void operateOn(struct VariableAttribute * node) {}
  virtual void operateOn(struct VariableDeclaration * node) {}
  virtual void operateOn(struct VariableList * node) {}
    
};


#endif
