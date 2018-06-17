#include "Types.h"
#include "Visitor.h"

Program * _root = nullptr;

void Assignment::visit(Visitor * v)
{
  v->operateOn(this);
}


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

void CastExpression::visit(Visitor * v)
{
  v->operateOn(this);
}

void CompareExpression::visit(Visitor * v)
{
  v->operateOn(this);
}

void ConstantExpression::visit(Visitor * v)
{
  v->operateOn(this);
}

void Define::visit(Visitor * v)
{
  v->operateOn(this);
}

void DeclarationSpecifier::visit(Visitor * v)
{
  v->operateOn(this);
}

void DeclarationSpecifierList::visit(Visitor * v)
{
  v->operateOn(this);
}

void Expression::visit(Visitor * v)
{
  v->operateOn(this);
}

void ForLoop::visit(Visitor * v)
{
  v->operateOn(this);
}

void FunctionCall::visit(Visitor * v)
{
  v->operateOn(this);
}

void FunctionCallArgumentList::visit(Visitor * v)
{
  v->operateOn(this);
}

void FunctionDeclaration::visit(Visitor * v)
{
  v->operateOn(this);
}

void LabeledStatement::visit(Visitor * v)
{
  v->operateOn(this);
}

void SelectionStatement::visit(Visitor * v)
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

void JumpStatement::visit(Visitor * v)
{
  v->operateOn(this);
}

void Statement::visit(Visitor * v)
{
  v->operateOn(this);
}

std::vector<struct VariableDeclaration*> Struct::getVariables() const
{
  std::vector<struct VariableDeclaration*> variables;
  
  const Block & block = _block;
  const std::vector<Node*> & nodes = block._nodes;
  for(Node * node : nodes) {
    if(node->getNodeType() == NodeType::VariableDeclaration) {
      variables.push_back(static_cast<VariableDeclaration*>(node));
    }
  }
  return variables;
}

void Struct::visit(Visitor * v)
{
  v->operateOn(this);
}

void TypeSpecifier::visit(Visitor *v)
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
