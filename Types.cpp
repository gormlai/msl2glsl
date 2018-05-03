#include "Types.h"
#include "Visitor.h"

Program * _root = nullptr;

void BinaryExpression::visit(Visitor * v)
{
	v->operateOn(this);
}

void Block::visit(Visitor * v)
{
  v->operateOn(this);
}

void BufferDescriptor::visit(Visitor * v)
{
  v->operateOn(this);
}

void ConstantExpression::visit(Visitor * v)
{
	v->operateOn(this);
}

void Expression::visit(Visitor * v)
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

void Statement::visit(Visitor * v)
{
  v->operateOn(this);
}

void Struct::visit(Visitor * v)
{
  v->operateOn(this);
}

void UnaryExpression::visit(Visitor * v)
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
