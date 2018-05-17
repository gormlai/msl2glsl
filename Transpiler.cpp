#include "Types.h"
#include "Transpiler.h"

std::string Transpiler::convert(struct Block * program, struct FunctionDeclaration * shader)
{
  _shaderString = std::string("");
  program->visit(this);
  return _shaderString;
}

void Transpiler::operateOn(struct AssignStatement * desc)
{
}

void Transpiler::operateOn(struct BinaryExpression * desc)
{
}

void Transpiler::operateOn(struct Block * block)
{
}

void Transpiler::operateOn(struct BufferDescriptor * desc)
{
}

void Transpiler::operateOn(struct ConstantExpression * desc)
{
}

void Transpiler::operateOn(struct Expression * desc)
{
}

void Transpiler::operateOn(struct FunctionCall * node)
{
}

void Transpiler::operateOn(struct FunctionCallArgumentList * node)
{
}

void Transpiler::operateOn(struct FunctionDeclaration * node)
{
}

void Transpiler::operateOn(struct Node * node)
{
}

void Transpiler::operateOn(struct Program * program)
{
}

void Transpiler::operateOn(struct ReturnStatement * statement)
{
}

void Transpiler::operateOn(struct Statement * statement)
{
}

void Transpiler::operateOn(struct Struct * strct)
{
}

void Transpiler::operateOn(struct UnaryExpression * desc)
{
}

void Transpiler::operateOn(struct UsingDeclaration * usingDecl)
{
}

void Transpiler::operateOn(struct VariableAttribute * node)
{
}

void Transpiler::operateOn(struct VariableDeclaration * node)
{
}

void Transpiler::operateOn(struct VariableList * node)
{
}
