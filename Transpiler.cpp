#include "Types.h"
#include "Transpiler.h"

#include <map>
#include <set>

namespace
{
  const std::set<std::string> g_simpleGLTypes = 
    {
      "float",
      "double",
      "int",
      "mat4",
      "uvec2",
      "uvec3",
      "uvec4",
      "ivec2",
      "ivec3",
      "ivec4",
      "vec2",
      "vec3",
      "vec4",
    };

  std::vector<Struct*> gatherStructs(Block * block)
  {
    std::vector<Struct*> result;
    
    std::vector<Node*> nodes = block->_nodes;
    for(auto node : nodes) {
      if(node->getNodeType() == NodeType::Struct) {
	Struct * strct = (Struct*)node;
	result.push_back(strct);
      }
    }
    return result;
    
  }
}

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
		{ "float4x4", "mat4" },
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
  _inDecl = nullptr;
  _topLevelStructs = ::gatherStructs(program); 

  const std::string mainString = outputMain();
  const std::string inOutUniforms = outputInOutUniforms();

  // add version marker - needs more flexibility in future versions
  _shaderString = _shaderString + "#version 430 core\n\n";

  program->visit(this);

  //  _shaderString += "\n" + inOutUniforms + "\n";  
  _shaderString += _shaderString + "\n" + mainString + "\n";
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

void Transpiler::categoriseVariableDeclaration(VariableDeclaration * vDecl)
{
  VariableAttribute * vAttrib = vDecl->_attribute;
  if(vAttrib != nullptr) {
    const std::string & sAttrib = vAttrib->_sAttribute;
    if(sAttrib == "stage_in") {
      _inDecl = vDecl;
    }
  }
}


bool Transpiler::isSimpleGLType(const std::string & glType) const
{
  auto it = g_simpleGLTypes.find(glType);
  return it != g_simpleGLTypes.end();
}

std::string Transpiler::outputInOutUniforms()
{
  std::string result;
  // handle in
  if(_inDecl != nullptr) {
    const std::string type = _inDecl->_type;
    const std::string mappedType = mapIdentifier(type);
    if(isSimpleGLType(mappedType))
      result += "in " + mappedType + " " + mapIdentifier(_inDecl->_variableName) + ";\n";
    else { // else search for struct
      
    }
      
  }

  

  return result;
}


std::string Transpiler::outputMain()
{
  std::string mainCode;
  
  // categorise arguments
  VariableList * vList = _shader->_variables;
  if(vList != nullptr) {
    std::vector<VariableDeclaration *> & vDecls = vList->_variableDeclarations;
    for(VariableDeclaration * vDecl : vDecls)
      categoriseVariableDeclaration(vDecl);
    
  }
  
  // convert shader
  mainCode = mainCode + "void main()\n";
  
  if (_shader->_block != nullptr)
    _shader->_block->visit(this);
  
  mainCode = mainCode + "\n\n";
  
  // analyze arguments to set in and out variables correctly

  return mainCode;
}

void Transpiler::operateOn(struct FunctionDeclaration * node)
{
	// we are only interested in one type of function - 
	// utility functions.
	// all other functions should be ignored
	if (node->_functionType == FunctionType::Utility)
	{
		if (!node->_returnType.empty())
			_shaderString = _shaderString + node->_returnType + " ";

		_shaderString = _shaderString + node->_name + "(";

		if (node->_variables != nullptr)
			node->_variables->visit(this);

		_shaderString = _shaderString + ")\n";

		if (node->_block != nullptr)
			node->_block->visit(this);

		_shaderString = _shaderString + "\n\n";

	}
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
	_shaderString = _shaderString + ";\n\n";
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
