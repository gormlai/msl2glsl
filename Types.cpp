#include "Types.h"
#include "Visitor.h"

Program * _root = nullptr;

void Block::visit(Visitor * v)
{
  v->operateOn(this);
}

void BufferDescriptor::visit(Visitor * v)
{
  v->operateOn(this);
}

void Declaration::visit(Visitor * v)
{
  v->operateOn(this);
}

void FunctionDeclaration::visit(Visitor * v)
{
  v->operateOn(this);
}

void Node::visit(Visitor * v)
{
  v->operateOn(this);
}

void Program::visit(Visitor * v)
{
  v->operateOn(this);
}

void Struct::visit(Visitor * v)
{
  v->operateOn(this);
}

void UsingDeclaration::visit(Visitor * v)
{
  v->operateOn(this);
}

void VariableAttribute::visit(Visitor * v)
{
  v->operateOn(this);
}

void VariableDeclaration::visit(Visitor * v)
{
  v->operateOn(this);
}

void VariableList::visit(Visitor * v)
{
  v->operateOn(this);
}
