#include "Types.h"
#include "Visitor.h"

Program * _root = nullptr;

void Node::visit(Visitor * v)
{
  v->preOperateOn(this);
  v->postOperateOn(this);
}

void Declaration::visit(Visitor * v)
{
  v->preOperateOn(this);
  v->postOperateOn(this);
}

void DeclarationList::visit(Visitor * v)
{
  v->preOperateOn(this);
  
  for(auto node : _nodes)
    node->visit(v);
  
  v->postOperateOn(this);
}

void UsingDeclaration::visit(Visitor * v)
{
  v->preOperateOn(this);
  v->postOperateOn(this);
}

void Struct::visit(Visitor * v)
{
  printf("Struct::visit\n");
  v->preOperateOn(this);
  v->postOperateOn(this);
}

void Program::visit(Visitor * v)
{
  v->preOperateOn(this);
  
  if(_decls!=nullptr)
    _decls->visit(v);
  
  v->postOperateOn(this);
}
