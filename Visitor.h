#ifndef _VISITOR_H_
#define _VISITOR_H_

class Visitor
{
 public:
  virtual ~Visitor() {}
  
  virtual void operateOn(struct Struct * strct) {}
  virtual void operateOn(struct UsingDeclaration * usingDecl) {}
  virtual void operateOn(struct Block * block) {}
  virtual void operateOn(struct Program * program) {}
  virtual void operateOn(struct Declaration * decl) {}
  virtual void operateOn(struct Node * node) {}
  virtual void operateOn(struct VariableAttribute * node) {}
  virtual void operateOn(struct UChar * node) {}  
  virtual void operateOn(struct UChar2 * node) {}  
  virtual void operateOn(struct UChar3 * node) {}  
  virtual void operateOn(struct UChar4 * node) {}  
  virtual void operateOn(struct Float * node) {}  
  virtual void operateOn(struct Float2 * node) {}
  virtual void operateOn(struct Float3 * node) {}  
  virtual void operateOn(struct Float4 * node) {}
  virtual void operateOn(struct VariableDeclaration * node) {}
    
};


#endif
