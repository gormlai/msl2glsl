#include "Types.h"
#include "Visitor.h"

Block * _root = nullptr;

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

void Block::visit(Visitor * v)
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
  v->preOperateOn(this);
  v->postOperateOn(this);
}

