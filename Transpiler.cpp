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

std::string Transpiler::indent()
{
  std::string result;
  for (int i = 0; i < _indent; i++)
    result = result + "  ";

  return result;
}

std::string Transpiler::traverse(struct Node * node)
{
  std::string result;
  
  switch(node->getNodeType())
    {
    case NodeType::AssignStatement:
      result = result + operateOn(static_cast<AssignStatement*>(node));
      break;
    case NodeType::BinaryExpression:
      result = result + operateOn(static_cast<BinaryExpression*>(node));
      break;
    case NodeType::Block:
      result = result + operateOn(static_cast<Block*>(node));
      break;
    case NodeType::BufferDescriptor:
      result = result + operateOn(static_cast<BufferDescriptor*>(node));
      break;
    case NodeType::ConstantExpression:
      result = result + operateOn(static_cast<ConstantExpression*>(node));
      break;
    case NodeType::Expression:
      result = result + operateOn(static_cast<Expression*>(node));
      break;
    case NodeType::FunctionCall:
      result = result + operateOn(static_cast<FunctionCall*>(node));
      break;
    case NodeType::FunctionCallArgumentList:
      result = result + operateOn(static_cast<FunctionCallArgumentList*>(node));
      break;
    case NodeType::FunctionDeclaration:
      result = result + operateOn(static_cast<FunctionDeclaration*>(node));
      break;
    case NodeType::Node:
      result = result + operateOn(static_cast<Node*>(node));
      break;
    case NodeType::Program:
      result = result + operateOn(static_cast<Program*>(node));
      break;
    case NodeType::ReturnStatement:
      result = result + operateOn(static_cast<ReturnStatement*>(node));
      break;
    case NodeType::Statement:
      result = result + operateOn(static_cast<Statement*>(node));
      break;
    case NodeType::Struct:
      result = result + operateOn(static_cast<Struct*>(node));
      break;
    case NodeType::UnaryExpression:
      result = result + operateOn(static_cast<UnaryExpression*>(node));
      break;
    case NodeType::UsingDeclaration:
      result = result + operateOn(static_cast<UsingDeclaration*>(node));
      break;
    case NodeType::VariableAttribute:
      result = result + operateOn(static_cast<VariableAttribute*>(node));
      break;
    case NodeType::VariableDeclaration:
      result = result + operateOn(static_cast<VariableDeclaration*>(node));
      break;
    case NodeType::VariableList:
      result = result + operateOn(static_cast<VariableList*>(node));
      break;      
    }
  
  return result;
}

std::string Transpiler::convert(struct Block * program, struct FunctionDeclaration * shader)
{
  std::string shaderString = std::string("");
  
  _indent = 0;
  _shader = shader;
  _inDecl = nullptr;
  _topLevelStructs = ::gatherStructs(program); 

  const std::string mainString = outputMain();  
  const std::string inOutUniforms = outputInOutUniforms();

  // add version marker - needs more flexibility in future versions
  shaderString = shaderString + "#version 430 core\n\n";

  shaderString += traverse(program);

  //  _shaderString += "\n" + inOutUniforms + "\n";  
  shaderString = shaderString + "\n" + mainString + "\n";
  return shaderString;
}

std::string Transpiler::operateOn(struct AssignStatement * desc)
{
  std::string result =  indent();
  result = result + traverse(desc->_left);
  result = result + " = ";
  result = result + traverse(desc->_right);
  return result;
}

std::string Transpiler::operateOn(struct BinaryExpression * desc)
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
  std::string result;
  result = result + traverse(desc->_left);
  result = result + ops[(int)desc->_op];
  result = result + traverse(desc->_right);
  return result;
}

std::string Transpiler::operateOn(struct Block * block)
{
  std::string result = indent();
  result = result + "{\n";
  
  _indent++;
  
  for (auto node : block->_nodes)
    {
      if (node == nullptr)
	continue;
      
      result = result + traverse(node);
      result = result + ";\n";
    }

  _indent--;

  result = result + indent();
  result = result + "}";
  return result;
}

std::string Transpiler::operateOn(struct BufferDescriptor * desc)
{
  return std::string("");
}

std::string Transpiler::operateOn(struct ConstantExpression * desc)
{
  std::string result;
  
  switch (desc->_type)
    {
    case ConstantType::Int:
      result = result + std::to_string(desc->_int);
      break;
    case ConstantType::Float:
      result = result + std::to_string(desc->_float) + "f";
		break;
    case ConstantType::Double:
      result = result + std::to_string(desc->_double);
      break;
    case ConstantType::Half:
      result = result + std::to_string(desc->_half) + "f";
      break;
    case ConstantType::Identifier:
      result = result + mapIdentifier(desc->_identifier);
      break;
    }

  return result;
}

std::string Transpiler::operateOn(struct Expression * desc)
{
  // Expression is a Base class. Don't do anything here
  std::string result;
  return result;
}

std::string Transpiler::operateOn(struct FunctionCall * node)
{
  std::string result;
  result = result + mapIdentifier(node->_name);
  result = result + "(";

  if (node->_arguments != nullptr)
    result = result + traverse(node->_arguments);
  
  result = result + ")";

  return result;
}

std::string Transpiler::operateOn(struct FunctionCallArgumentList * node)
{
  std::string result;
  
  const unsigned int size = (unsigned int)node->_expressions.size();
  for (unsigned int i = 0; i < size; i++) {
    
    Expression * e = node->_expressions[i];
    result = result + traverse(e);

    if (i != size - 1)
      result = result + ",";
  }

  return result;
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
  
  //  if (_shader->_block != nullptr)
  mainCode += traverse(_shader->_block);
  
  mainCode = mainCode + "\n\n";
  
  // analyze arguments to set in and out variables correctly

  return mainCode;
}

std::string Transpiler::operateOn(struct FunctionDeclaration * node)
{
  std::string result;
  
  // we are only interested in one type of function - 
  // utility functions.
  // all other functions should be ignored
  if (node->_functionType == FunctionType::Utility)
    {
      if (!node->_returnType.empty())
	result = result + node->_returnType + " ";
      
      result = result + node->_name + "(";
      
      if (node->_variables != nullptr)
	result += traverse(node->_variables);
      
      result = result + ")\n";

      if (node->_block != nullptr)
	result += traverse(node->_block);
      
      result = result + "\n\n";
      
    }

  return result;
}

std::string Transpiler::operateOn(struct Node * node)
{
  // Node is a base class. Implementation should be in base classes
  std::string result;
  return result;
}

std::string Transpiler::operateOn(struct Program * program)
{
  std::string result;
  
  for (auto node : program->_nodes)
    result = result + traverse(node);

  return result;
}

std::string Transpiler::operateOn(struct ReturnStatement * statement)
{
  std::string result =  indent();
  result = result + "return ";
  result += traverse(statement->_expression);
  result = result + ";\n";

  return result;
}

std::string Transpiler::operateOn(struct Statement * statement)
{
  // Statement is a base class. Implementation should be in base classes  
  std::string result;
  return result;
}

std::string Transpiler::operateOn(struct Struct * strct)
{
  std::string result = indent();
  result = result + "struct " + strct->_name + "\n";

  result += traverse(&strct->_block);

  result += indent();
  result = result + ";\n\n";

  return result;
}

std::string Transpiler::operateOn(struct UnaryExpression * desc)
{
  std::string result;
  
  // pre expression
  switch (desc->_type)
    {
    case UnaryType::Minus:
      result = result + "-";
      break;
    case UnaryType::Parenthesis:
      result = result + "(";
      break;
    }
  
  result = result + traverse(desc->_expression);
  
  // post expression
  switch (desc->_type)
    {
    case UnaryType::Minus:
      result = result + "-";
      break;
    case UnaryType::Parenthesis:
      result = result + ")";
      break;
    }

  return result;
}

std::string Transpiler::operateOn(struct UsingDeclaration * usingDecl)
{
  // ignore for now. This needs special handling
  std::string result;
  return result;
}

std::string Transpiler::operateOn(struct VariableAttribute * node)
{
  // ignore for now. This needs special handling
  std::string result;
  return result;
}

std::string Transpiler::operateOn(struct VariableDeclaration * node)
{
  std::string result = indent();
  
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
  result = result + realType + " " + realVariableName;
  return result;
}

std::string Transpiler::operateOn(struct VariableList * node)
{
  std::string result;
  const int count = (int)node->_variableDeclarations.size();
  for (int i = 0; i < count; i++) {
    result += traverse(node->_variableDeclarations[i]);
    
    if (i != count - 1)
      result = result + ", ";
  }
  return result;

}
