#ifndef _VISITOR_H_
#define _VISITOR_H_

class Visitor
{
 public:
  virtual ~Visitor() {}
  
  virtual void preOperateOn(struct Struct * strct) {}
  virtual void preOperateOn(struct UsingDeclaration * usingDecl) {}
  virtual void preOperateOn(struct Block * block) {}
  virtual void preOperateOn(struct Declaration * decl) {}
  virtual void preOperateOn(struct Node * node) {}
  
  virtual void postOperateOn(struct Struct * strct) {}
  virtual void postOperateOn(struct UsingDeclaration * usingDecl) {}
  virtual void postOperateOn(struct Block * block) {}
  virtual void postOperateOn(struct Declaration * decl) {}
  virtual void postOperateOn(struct Node * node) {}
  
};


#endif
