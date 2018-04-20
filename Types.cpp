#include "Types.h"
#include "Visitor.h"

Program * _root = nullptr;

void Node::visit(Visitor * v)
{
  v->operateOn(this);
}

void Declaration::visit(Visitor * v)
{
  v->operateOn(this);
}

void DeclarationList::visit(Visitor * v)
{
  v->operateOn(this);
  for(auto node : _nodes)
    node->visit(v);
}

void UsingDeclaration::visit(Visitor * v)
{
  v->operateOn(this);
}

void Struct::visit(Visitor * v)
{
  v->operateOn(this);
}

void Program::visit(Visitor * v)
{
  v->operateOn(this);
}
