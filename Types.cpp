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

void Block::visit(Visitor * v)
{
  v->operateOn(this);
}

void Program::visit(Visitor * v)
{
  v->operateOn(this);
}

void UsingDeclaration::visit(Visitor * v)
{
  v->operateOn(this);
}

void Struct::visit(Visitor * v)
{
  v->operateOn(this);
}

void UChar::visit(Visitor * v)
{
  v->operateOn(this);
}

void UChar2::visit(Visitor * v)
{
  v->operateOn(this);
}

void UChar3::visit(Visitor * v)
{
  v->operateOn(this);
}

void UChar4::visit(Visitor * v)
{
  v->operateOn(this);
}

void Float::visit(Visitor * v)
{
  v->operateOn(this);
}

void Float2::visit(Visitor * v)
{
  v->operateOn(this);
}

void Float3::visit(Visitor * v)
{
  v->operateOn(this);
}

void Float4::visit(Visitor * v)
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
