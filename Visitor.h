#ifndef _VISITOR_H_
#define _VISITOR_H_

class Visitor
{
 public:
  virtual ~Visitor() {}
  
  virtual void operateOn(struct AssignStatement * desc)=0;  
  virtual void operateOn(struct BinaryExpression * desc)=0;  
  virtual void operateOn(struct Block * block) =0;
  virtual void operateOn(struct BufferDescriptor * desc)=0;
  virtual void operateOn(struct ConstantExpression * desc)=0;
  virtual void operateOn(struct Expression * desc)=0;  
  virtual void operateOn(struct FunctionCall * node)=0;
  virtual void operateOn(struct FunctionCallArgumentList * node)=0;
  virtual void operateOn(struct FunctionDeclaration * node)=0;
  virtual void operateOn(struct Node * node)=0;
  virtual void operateOn(struct Program * program)=0;
  virtual void operateOn(struct ReturnStatement * statement)=0;
  virtual void operateOn(struct Statement * statement)=0;
  virtual void operateOn(struct Struct * strct)=0;
  virtual void operateOn(struct UnaryExpression * desc)=0;
  virtual void operateOn(struct UsingDeclaration * usingDecl)=0;
  virtual void operateOn(struct VariableAttribute * node)=0;
  virtual void operateOn(struct VariableDeclaration * node)=0;
  virtual void operateOn(struct VariableList * node)=0;
    
};


#endif
