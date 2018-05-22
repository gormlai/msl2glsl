#include "Types.h"
#include "Transpiler.h"

#include <map>

std::string Transpiler::mapIdentifier(const std::string & src) const
{
	const static std::map<std::string, std::string> identifierMap =
	{
		{ "in", "in_" },
		{ "out", "out_"},
		{ "uchar2", "uvec2" },
		{ "uchar3", "uvec3" },
		{ "uchar4", "uvec4" },
		{ "float2", "vec2" },
		{ "float3", "vec3" },
		{ "float4", "vec4" },
	};

	std::string output = src;

	auto it = identifierMap.find(src);
	if (it != identifierMap.end())
		output = it->second;

	return output;
}

void Transpiler::indent()
{
	for (int i = 0; i < _indent; i++)
		_shaderString = _shaderString + "  ";

}

std::string Transpiler::convert(struct Block * program, struct FunctionDeclaration * shader)
{
  _shaderString = std::string("");
	_indent = 0;
	_shader = shader;
  program->visit(this);
  return _shaderString;
}

void Transpiler::operateOn(struct AssignStatement * desc)
{
	indent();
	desc->_left->visit(this);
	_shaderString = _shaderString + " = ";
	desc->_right->visit(this);
}

void Transpiler::operateOn(struct BinaryExpression * desc)
{
	const static std::string ops[] =
	{
		"+",
		"-",
		"*",
		"/",
		".",
	};

	// special attention needs to be to the dot operator - will come back to this
	desc->_left->visit(this);
	_shaderString = _shaderString + ops[(int)desc->_op];
	desc->_right->visit(this);
}

void Transpiler::operateOn(struct Block * block)
{
	indent();
	_shaderString = _shaderString + "{\n";

	_indent++;

	for (auto node : block->_nodes)
	{
		if (node == nullptr)
			continue;

		node->visit(this);
		_shaderString = _shaderString + ";\n";
	}

	_indent--;

	indent();
	_shaderString = _shaderString + "}";
}

void Transpiler::operateOn(struct BufferDescriptor * desc)
{
}

void Transpiler::operateOn(struct ConstantExpression * desc)
{
	switch (desc->_type)
	{
	case ConstantType::Int:
		_shaderString = _shaderString + std::to_string(desc->_int);
		break;
	case ConstantType::Float:
		_shaderString = _shaderString + std::to_string(desc->_float) + "f";
		break;
	case ConstantType::Double:
		_shaderString = _shaderString + std::to_string(desc->_double);
		break;
	case ConstantType::Half:
		_shaderString = _shaderString + std::to_string(desc->_half) + "f";
		break;
	case ConstantType::Identifier:
		_shaderString = _shaderString + mapIdentifier(desc->_identifier);
		break;
	}

}

void Transpiler::operateOn(struct Expression * desc)
{
	// Expression is a Base class. Don't do anything here
}

void Transpiler::operateOn(struct FunctionCall * node)
{
	_shaderString = _shaderString + mapIdentifier(node->_name);
	_shaderString = _shaderString + "(";

	if (node->_arguments != nullptr)
		node->_arguments->visit(this);

	_shaderString = _shaderString + ")";

}

void Transpiler::operateOn(struct FunctionCallArgumentList * node)
{
	const unsigned int size = (unsigned int)node->_expressions.size();
	for (unsigned int i = 0; i < size; i++) {

		Expression * e = node->_expressions[i];
		e->visit(this);

		if (i != size - 1)
			_shaderString = _shaderString + ",";
	}
}

void Transpiler::operateOn(struct FunctionDeclaration * node)
{
	// we are only interested in two types of functions - 
	// the shader that we need to convert OR
	// utility functions.
	// all other functions should be ignored

	if (node == _shader)
	{
		// convert shader
		_shaderString = _shaderString + "void main()";

		if (node->_block != nullptr)
			node->_block->visit(this);

		_shaderString = _shaderString + "\n";

		// analyze arguments to set in and out variables correctly

	}
	else if (node->_functionType == FunctionType::Utility)
	{
		if (!node->_returnType.empty())
			_shaderString = _shaderString + node->_returnType + " ";

		_shaderString = _shaderString + node->_name + "(";

		if (node->_variables != nullptr)
			node->_variables->visit(this);

		_shaderString = _shaderString + ")\n";

		if (node->_block != nullptr)
			node->_block->visit(this);

		_shaderString = _shaderString + "\n";

	}
	else
		return;
}

void Transpiler::operateOn(struct Node * node)
{
	// Node is a base class. Implementation should be in base classes
}

void Transpiler::operateOn(struct Program * program)
{
	for (auto node : program->_nodes)
		node->visit(this);
}

void Transpiler::operateOn(struct ReturnStatement * statement)
{
	indent();
	_shaderString = _shaderString + "return ";
	statement->_expression->visit(this);
	_shaderString = _shaderString + ";\n";
}

void Transpiler::operateOn(struct Statement * statement)
{
	// Statement is a base class. Implementation should be in base classes  
}

void Transpiler::operateOn(struct Struct * strct)
{
	indent();
	_shaderString = _shaderString + "struct " + strct->_name + "\n";

	strct->_block.visit(this);

	indent();
	_shaderString = _shaderString + ";\n";
}

void Transpiler::operateOn(struct UnaryExpression * desc)
{
	// pre expression
	switch (desc->_type)
	{
	case UnaryType::Minus:
		_shaderString = _shaderString + "-";
		break;
	case UnaryType::Parenthesis:
		_shaderString = _shaderString + "(";
		break;
	}

	desc->_expression->visit(this);

	// post expression
	switch (desc->_type)
	{
	case UnaryType::Minus:
		_shaderString = _shaderString + "-";
		break;
	case UnaryType::Parenthesis:
		_shaderString = _shaderString + ")";
		break;
	}
}

void Transpiler::operateOn(struct UsingDeclaration * usingDecl)
{
	// ignore for now. This needs special handling
}

void Transpiler::operateOn(struct VariableAttribute * node)
{
	// ignore for now. This needs special handling
}

void Transpiler::operateOn(struct VariableDeclaration * node)
{
	indent();

	// ignore all qualifiers
	std::string type = node->_type;
	BufferDescriptor * bufDesc = node->_bufferDescriptor;
	if (bufDesc != nullptr)
	{

		if (node->_type == "texture2d"){
			if (bufDesc->_type == "float" && bufDesc->_accessor == "sample") {
				type = "sampler2D";
			}

		}
	}

	std::string realType = mapIdentifier(type);
	std::string realVariableName = mapIdentifier(node->_variableName);
	_shaderString = _shaderString + realType + " " + realVariableName;

}

void Transpiler::operateOn(struct VariableList * node)
{
	const int count = (int)node->_variableDeclarations.size();
	for (int i = 0; i < count; i++) {
		node->_variableDeclarations[i]->visit(this);

		if (i != count - 1)
			_shaderString = _shaderString + ", ";
	}

}
