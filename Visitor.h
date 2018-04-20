#ifndef _VISITOR_H_
#define _VISITOR_H_

class Visitor
{
 public:
  virtual void operateOn(struct Program * program) {}
  virtual void operateOn(struct Struct * strct) {}
  virtual void operateOn(struct UsingDeclaration * usingDecl) {}
  virtual void operateOn(struct DeclarationList * declList) {}
  virtual void operateOn(struct Declaration * decl) {}
  virtual void operateOn(struct Node * node) {}
  
};


#endif
