#include "Types.h"
#include "Transpiler.h"

#include <map>
#include <set>
#include <iostream>

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
      "sampler2D",
      "sampler3D",
      "usampler2D",
      "usampler3D",
      "isampler2D",
      "isampler3D",
    };
  
  const std::set<std::string> g_samplerGLTypes = 
    {
      "sampler2D",
      "sampler3D",
      "usampler2D",
      "usampler3D",
      "isampler2D",
      "isampler3D",
    };

  std::map<std::string, Struct*> gatherStructs(Block * block)
  {
    std::map<std::string, Struct*> result;
    
    std::vector<Node*> nodes = block->_nodes;
    for(auto node : nodes) {
      if(node->getNodeType() == NodeType::Struct) {
	Struct * strct = (Struct*)node;
	const std::string & strctName = strct->_name;
	result[strctName] = strct;
      }
    }
    return result;
    
  }
}

std::string Transpiler::toCommaSeparatedList(const std::vector<VariableNameDeclaration*> & input, bool mapIdentifiers)
{
  std::string result;
  
  for(unsigned int i=0 ; i < (unsigned int)input.size() ; i++) {
    const VariableNameDeclaration * vDecl = input[i];
    const std::string val = mapIdentifiers ? mapIdentifier(vDecl->_variableName) : vDecl->_variableName;
    result = result + val;
    if(i != input.size()-1)
      result = result + ", ";
  }
  return result;
}


std::string Transpiler::mapStructMember(const std::string & possibleStructMember) const
{
  auto it = _structMemberMap.find(possibleStructMember);
  if(it == _structMemberMap.end())
    return possibleStructMember;

  return it->second;
}

std::string Transpiler::mapToGLType(const VariableDeclaration * vDecl) const
{
  DeclarationSpecifierList * declList = vDecl->_declarationSpecifiers;
  std::string result;

  if(declList != nullptr)
    result = result + mapToGLType(declList, vDecl->_bufferDescriptor);

  return result;
}

std::string Transpiler::mapToGLType(const DeclarationSpecifierList * declSpecList, const BufferDescriptor * bufDesc) const
{
  std::string result;
  if(declSpecList != nullptr) {
    
    for(const DeclarationSpecifier * declSpec : declSpecList->_specifiers)
      result = result + mapToGLType(declSpec, bufDesc);

  }

  return result;
}

std::string Transpiler::mapToGLType(const DeclarationSpecifier * declSpec, const BufferDescriptor * bufDesc) const
{
  std::string result = "Unknown DeclarationSpecifier";
  switch(declSpec->getNodeType())
    {
    case NodeType::TypeSpecifier:
      result = mapToGLType(static_cast<const TypeSpecifier*>(declSpec), bufDesc);
      break;
    case NodeType::Qualifier:
      result = mapToGLType(static_cast<const Qualifier*>(declSpec), bufDesc);
    default:
      break;
  }

  return result;

}

std::string Transpiler::mapToGLType(const Qualifier * qualifier, const BufferDescriptor * bufDesc) const
{
  switch(qualifier->_type) {
  case QualifierType::Constant:
    return "";
    break;
  case QualifierType::Const:
    return "";
    break;
  case QualifierType::Constexpr:
    return "";
    break;
  case QualifierType::Device:
    return "";
    break;
  case QualifierType::Signed:
    return "";
    break;
  case QualifierType::Unsigned:
    return "unsigned";
    break;
  case QualifierType::Static:
    return "";
    break;
  }

  return std::string("Unknown Qualifier\n");
}


std::string Transpiler::mapToGLType(const TypeSpecifier * typeSpec, const BufferDescriptor * bufDesc) const
{
  std::string result = "unknown type";
  if(typeSpec != nullptr) {
    switch(typeSpec->_type)
      {
      case ETypeSpecifier::Int:
	result = "int";
	break;
      case ETypeSpecifier::Custom:
	result = typeSpec->_customTypeName;
	break;
      default:
	break;
      } 
    
  }
  
  if(bufDesc != nullptr) {
    if(result == "texture2d" && bufDesc->_accessor == "sample") {
      if(bufDesc->_type == "float")
	result = "sampler2D";
    }
  }

  return mapIdentifier(result);

}


std::string Transpiler::mapIdentifier(const std::string & src) const
{
	const static std::map<std::string, std::string> identifierMap =
	{
		{ "in", "in_" },
		{ "out", "out_"},
		{ "float2", "vec2" },
		{ "float3", "vec3" },
		{ "float4", "vec4" },
		{ "float4x4", "mat4" },
		{ "half2", "vec2" },
		{ "half3", "vec3" },
		{ "half4", "vec4" },
		{ "matrix_float4x4", "mat4" },
		{ "packed_float3", "vec3" },
		{ "uchar2", "uvec2" },
		{ "uchar3", "uvec3" },
		{ "uchar4", "uvec4" },
		{ "uint2", "uvec2" },
		{ "uint3", "uvec3" },
		{ "uint4", "uvec4" },
	};

	std::string output = src;

	auto it = identifierMap.find(src);
	if (it != identifierMap.end())
		output = it->second;

	return output;
}

// TODO: eventually this method should include scoped variables
// for now it only supports inputs
VariableDeclaration * Transpiler::getVariableFromName(const std::string & name)
{
  // categorise arguments to main
  VariableList * vList = _shader->_variables;
  if(vList != nullptr) {
    std::vector<VariableDeclaration *> & vDecls = vList->_variableDeclarations;
    for(VariableDeclaration * vDecl : vDecls) {
      for(const VariableNameDeclaration * variableName : vDecl->_variableNames) {
      if(name == variableName->_variableName)
	return vDecl;
      }
    }
    
  }

  return nullptr;
  
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
    case NodeType::Assignment:
      result = result + operateOn(static_cast<Assignment*>(node));
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
    case NodeType::CastExpression:
      result = result + operateOn(static_cast<CastExpression*>(node));
      break;
    case NodeType::CompareExpression:
      result = result + operateOn(static_cast<CompareExpression*>(node));
      break;
    case NodeType::ConstantExpression:
      result = result + operateOn(static_cast<ConstantExpression*>(node));
      break;
    case NodeType::DeclarationSpecifier:
      result = result + operateOn(static_cast<DeclarationSpecifier*>(node));
      break;
    case NodeType::DeclarationSpecifierList:
      result = result + operateOn(static_cast<DeclarationSpecifierList*>(node));
      break;
    case NodeType::Expression:
      result = result + operateOn(static_cast<Expression*>(node));
      break;
    case NodeType::JumpStatement:
      result = result + operateOn(static_cast<JumpStatement*>(node));
      break;
    case NodeType::ForLoop:
      result = result + operateOn(static_cast<ForLoop*>(node));
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
    case NodeType::LabeledStatement:
      result = result + operateOn(static_cast<LabeledStatement*>(node));
      break;
    case NodeType::Node:
      result = result + operateOn(static_cast<Node*>(node));
      break;
    case NodeType::Preprocessor:
      result = result + operateOn(static_cast<Preprocessor*>(node));
      break;
    case NodeType::Program:
      result = result + operateOn(static_cast<Program*>(node));
      break;
    case NodeType::Qualifier:
      result = result + operateOn(static_cast<Qualifier*>(node));
      break;
    case NodeType::SelectExpression:
      result = result + operateOn(static_cast<SelectExpression*>(node));
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

  _inLocation = 0;
  _outLocation = 0;
  _uniformLocation = 0;
  _indent = 0;
  _shader = shader;
  _inDecl = nullptr;
  _topLevelStructs = ::gatherStructs(program);
  
  _state = TranspilerState::Init;
  shaderString = shaderString + "#version 430 core\n\n";

  // add version marker - needs more flexibility in future versions
  _state = TranspilerState::OutputGlobals;
  const std::string inOutUniforms = outputInOutUniforms();

  _state = TranspilerState::OutputMain;
  const std::string mainString = outputMain();

  _state = TranspilerState::OutputRestOfProgram;
  const std::string mainProgram = traverse(program);

  _state = TranspilerState::CleaningUp;
  shaderString = shaderString + "\n" + inOutUniforms + "\n";  
  shaderString += mainProgram;
  shaderString = shaderString + "\n" + mainString + "\n";
  return shaderString;
}

std::string Transpiler::operateOn(struct Assignment * desc)
{
  std::string result =  indent();
  result = result + traverse(desc->_left);
  result = result + " = ";
  result = result + traverse(desc->_right);
  return result;
}

std::string Transpiler::operateOn(struct BinaryExpression * desc)
{
  std::string result;

  const static std::string ops[] =
    {
      "+",
      "-",
      "*",
      "/",
      ".",
      "->",
      "",
      "|",
      "||",
      "&",
      "&&",
      "<<",
      ">>",
      "",
      "^",
      "%",
    };

  switch(desc->_op){
  case BinaryOperator::Plus:
  case BinaryOperator::Minus:
  case BinaryOperator::Multiply:
  case BinaryOperator::Divide:
  case BinaryOperator::BinaryAnd:
  case BinaryOperator::LogicalAnd:
  case BinaryOperator::BinaryOr:
  case BinaryOperator::LogicalOr:
  case BinaryOperator::LeftShift:
  case BinaryOperator::RightShift:
  case BinaryOperator::XOr:
  case BinaryOperator::Modulo:
    result = result + traverse(desc->_left);
    result = result + ops[(int)desc->_op];
    result = result + traverse(desc->_right);
    break;
  case BinaryOperator::Cast:
    result = result + "(";
    result = result + traverse(desc->_left);
    result = result + ")";
    result = result + traverse(desc->_right);
    break;
  case BinaryOperator::Index:
    result = result + traverse(desc->_left);
    result = result + "[";
    result = result + traverse(desc->_right);
    result = result + "]";
    break;
  case BinaryOperator::Dot: 
  case BinaryOperator::Pointer: {
    // special attention needs to be to the dot operator
    const std::string rightExpression = traverse(desc->_right);
    const std::string dot = ops[(int)desc->_op];
    
    const std::string leftExpression = traverse(desc->_left);
    VariableDeclaration * vDecl = getVariableFromName(leftExpression);
    if(vDecl == nullptr) {
      result = leftExpression + dot + rightExpression;
    }
    else {
      const std::string glType = mapToGLType(vDecl);
      if(glType == "sampler2D") {
	const std::string phraseToSearchFor = "sample(";
	std::size_t samplePos = rightExpression.find(phraseToSearchFor);
	if(samplePos == 0) {
	  std::string intermediate = rightExpression.substr(phraseToSearchFor.length()-1);
	  // TODO - replace the following code with a search for the correction VariableDeclaration
	  std::size_t commaPos = intermediate.find(",");
	  if(commaPos != std::string::npos) {
	    std::string rightSide = intermediate.substr(commaPos);	    
	    result = "texture(" + mapIdentifier(leftExpression) + rightSide;
	  }
	}
      }
      else {
	const std::string structExpr = leftExpression + dot + rightExpression;
	const std::string mappedStructExpr = mapStructMember(structExpr);
	result = result + mappedStructExpr;
      }
      
    }
    
  }
    break;
  }
  
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

std::string Transpiler::operateOn(struct CastExpression * desc)
{
  // glsl doesn't support casts so we have to reformat it as a constructor
  std::string result;
  
  if(desc != nullptr) {
    result += desc->_castTo + "(";
    
    if(desc->_right!=nullptr)
      result += traverse(desc->_right);

    result += ")";
  }
  return result;
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
    case ConstantType::Hex:
      result = result + desc->_identifier;
      break;
    }

  return result;
}

std::string Transpiler::operateOn(struct DeclarationSpecifier * desc)
{
  // DeclarationSpecifier is a Base class. Don't do anything here
  std::string result;
  return result;
}

std::string Transpiler::operateOn(struct DeclarationSpecifierList * descList)
{
  std::string result;
  std::vector<DeclarationSpecifier*> specifiers = descList->_specifiers;
  for(DeclarationSpecifier * spec : specifiers)
    result = result + traverse(spec);

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

std::string Transpiler::operateOn(struct LabeledStatement * statement)
{
  std::string result;

  switch(statement->_type) {
  case LabeledStatementType::Case:
    result = result + indent();
    result = result + "case ";
    result = result + traverse(statement->_label);
    result = result + ":\n";
    result = result + indent();
    result = result + traverse(statement->_statement);
    result = result + "\n";
    break;
  case LabeledStatementType::Default:
    result = result + indent();
    result = result + "default:\n";
    result = result + indent();
    result = result + traverse(statement->_statement);
    result = result + "\n";
    break;
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
    else
      _uniformVariables.push_back(vDecl);
  }
}

std::string  Transpiler::extractAttributeIndex(VariableDeclaration * vDecl)
{
  std::string result;
  if(vDecl!=nullptr && vDecl->_attribute!=nullptr) {
    VariableAttribute * attribute = vDecl->_attribute;
    Expression * e = attribute->_eAttribute;
    if(e!=nullptr) {
      result = traverse(e);
    }
  }
  return result;

}

bool Transpiler::isSamplerGLType(const std::string & glType) const
{
  auto it = g_samplerGLTypes.find(glType);
  return it != g_samplerGLTypes.end();
}

bool Transpiler::isSimpleGLType(VariableDeclaration * vDecl) const
{
  const std::string mappedType = mapToGLType(vDecl);
  return isSimpleGLType(mappedType);
}


bool Transpiler::isSimpleGLType(const std::string & glType) const
{
  auto it = g_simpleGLTypes.find(glType);
  return it != g_simpleGLTypes.end();
}

std::string Transpiler::outputUniforms()
{
  std::string insideBlock;
  std::string outsideBlock;
  bool hasInside = false;

  insideBlock = insideBlock + std::string("layout(std140) uniform ShaderBlock\n{\n");

  for(VariableDeclaration * decl : _uniformVariables) {
    const std::string attributeIndex = extractAttributeIndex(decl);    
    for(const VariableNameDeclaration * variableName : decl->_variableNames) {
      std::string glType = mapToGLType(decl);
      if(isSimpleGLType(glType)) {
	if(isSamplerGLType(glType)) {
	  if(!attributeIndex.empty())
	    outsideBlock = outsideBlock + std::string("layout(binding = ") + attributeIndex + ") ";
	  outsideBlock = outsideBlock + "uniform " + glType + " " + variableName->_variableName + ";\n";
	}
	else {
	  hasInside = true;
	  insideBlock = insideBlock + "  " + glType + " " + variableName->_variableName + ";\n";
	}
      }
      else {
      }
    }
  }
  insideBlock = insideBlock + std::string("};\n");

  std::string result = outsideBlock;
  if(hasInside)
    result = result + insideBlock;
  return result;
}

std::string Transpiler::addLocation(unsigned int & location)
{
  std::string result = std::string("layout(location = ") + std::to_string(location) + ")";
  location++;
  return result;
}

std::string Transpiler::outputIn()
{
  std::string result;
  
  // handle in
  if(_inDecl != nullptr) {
    const std::string mappedType = mapToGLType(_inDecl);
    for(const VariableNameDeclaration * variableName : _inDecl->_variableNames) {
      const std::string mappedName = mapIdentifier(variableName->_variableName);
      if(isSimpleGLType(mappedType))
	result += addLocation(_outLocation) + " in " + mappedType + " " + mappedName + ";\n";
      else { // else search for struct
	auto it = _topLevelStructs.find(mappedType);
	if(it != _topLevelStructs.end()) {
	  Struct * strct = it->second;
	  std::vector<VariableDeclaration*> variables = strct->getVariables();
	  for(VariableDeclaration * variable : variables) {
	      const std::string mappedMemberType = mapToGLType(variable);	  
	      for(const VariableNameDeclaration * mappedVariableName : variable->_variableNames) {
		const std::string mappedMemberName = mapIdentifier(mappedVariableName->_variableName);
		const std::string srcMappedStructVariableName = mappedName + "." + mappedMemberName;
		const std::string dstMappedStructVariableName = mappedName + "_" + mappedMemberName;
		_structMemberMap[srcMappedStructVariableName] = dstMappedStructVariableName;	  
		
		result += addLocation(_outLocation) + " in " + mappedMemberType + " " + dstMappedStructVariableName + ";\n";
	      }
	    }
	    
	}
	else {
	  // we could not find a type to match
	  // TODO - throw error!!
	}
      
      
      }
    }
      
  }

  return result;
}

std::string Transpiler::outputOut()
{
  std::string result;
  
  std::string type = mapToGLType(_shader->_declarationSpecifiers, nullptr);
  std::string mappedType = mapIdentifier(type);

  std::string mappedName = baseOutVariableName();

  if(isSimpleGLType(mappedType))
    result += addLocation(_inLocation) +  " out " + mappedType + " " + mappedName + ";\n";
  else { // else search for struct
    auto it = _topLevelStructs.find(type);
    if(it != _topLevelStructs.end()) {
      Struct * strct = it->second;
      std::vector<VariableDeclaration*> variables = strct->getVariables();
      for(VariableDeclaration * variable : variables) {
	const std::string & memberType = mapToGLType(variable);
	const std::string mappedMemberType = mapIdentifier(memberType);	  
	for(const VariableNameDeclaration * mappedVariableName : variable->_variableNames) {
	  const std::string mappedMemberName = mapIdentifier(mappedVariableName->_variableName);
	  const std::string srcMappedStructVariableName = mappedName + "." + mappedMemberName;
	  const std::string dstMappedStructVariableName = mappedName + "_" + mappedMemberName;
	  _structMemberMap[srcMappedStructVariableName] = dstMappedStructVariableName;	  
	
	  result += addLocation(_inLocation) + " out " + mappedMemberType + " " + dstMappedStructVariableName + ";\n";
	}
      }
    }
    else {
      // we could find a type to match
      // TODO - throw error!!
    }    
  }

  return result;
  
}

std::string Transpiler::outputInOutUniforms()
{
  std::string result;
  
  // categorise arguments to main
  VariableList * vList = _shader->_variables;
  if(vList != nullptr) {
    std::vector<VariableDeclaration *> & vDecls = vList->_variableDeclarations;
    for(VariableDeclaration * vDecl : vDecls)
      categoriseVariableDeclaration(vDecl);
    
  }

  std::string inVars = outputIn();
  std::string outVars = outputOut();
  std::string uniformVars = outputUniforms();
  
  result = result + "\n" + inVars;
  result = result + "\n" + outVars;
  result = result + "\n" + uniformVars;
  
  return result;
}


std::string Transpiler::outputMain()
{
  std::string mainCode;
  
  
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
      if (node->_declarationSpecifiers != nullptr)
	result = result + mapToGLType(node->_declarationSpecifiers, nullptr) + " ";
      
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

std::string Transpiler::operateOn(struct Qualifier * qualifier)
{
  std::string result;
  switch(qualifier->_type) {
  case QualifierType::Constant:
    break;
  case QualifierType::Const:
    break;
  case QualifierType::Constexpr:
    break;
  case QualifierType::Device:
    break;
  case QualifierType::Signed:
    result = "signed";
    break;    
  case QualifierType::Unsigned:
    result = "unsigned";
    break;
  case QualifierType::Static:
    break;
    }
  

  return result;
}

std::string Transpiler::operateOn(struct SelectExpression * exp)
{
  std::string result;

  result = result + indent();
  result = result + traverse(exp->_left);
  result = result + " ? ";
  result = result + traverse(exp->_middle);
  result = result + " : ";
  result = result + traverse(exp->_right);  

  return result;
}

  
std::string Transpiler::operateOn(struct JumpStatement * statement)
{
  std::string result;

  std::string rightSide = traverse(statement->_expression);
  
  if(_state == TranspilerState::OutputRestOfProgram) {
    // keep code as is
    std::string result =  indent();
    result = result + "return ";
    result += rightSide;
    result = result + ";\n";
  }
  else if(_state == TranspilerState::OutputMain) {
    // rewrite code to assign to an out variable
    const std::string type =  mapToGLType(_shader->_declarationSpecifiers, nullptr);
    const std::string mappedType = mapIdentifier(type);
    if(isSimpleGLType(mappedType)) {
      result = indent() + result + baseOutVariableName() + " = " + rightSide + ";\n"; 
    }
    else {
      std::string tempVariableName = baseOutVariableName() + "_temp_";
      // assign right hand side result to an intermediate variable
      result = indent() + mappedType + " " + tempVariableName + " = " + rightSide + ";\n";

      auto it = _topLevelStructs.find(mappedType);
      if(it != _topLevelStructs.end()) {
	Struct * strct = it->second;
	std::vector<VariableDeclaration*> variables = strct->getVariables();
	for(VariableDeclaration * variable : variables) {
	  for(const VariableNameDeclaration * variableName : variable->_variableNames) {
	    const std::string mappedMemberName = mapIdentifier(variableName->_variableName);
	    const std::string rightHandSide = tempVariableName + "." + mappedMemberName;
	    std::string leftHandSide;
	  
	    if(_shader->_functionType == FunctionType::Vertex && variable->_attribute!=nullptr && variable->_attribute->_sAttribute == "position") {
	      leftHandSide = "gl_Position";
	    }
	    else {
	      leftHandSide = baseOutVariableName() + "_" + mappedMemberName;
	    }
	  
	    result += indent() + leftHandSide + " = " + rightHandSide + ";\n";
	  }
      }
    }
      
    }
  }
  else {
    // todo - handle error
  }
  

  return result;
}

std::string Transpiler::operateOn(struct Statement * statement)
{
  // Statement is a base class. Implementation should be in base classes  
  std::string result;
  return result;
}

std::string Transpiler::createStructInitializer(struct Struct * strct, const std::string & initializations)
{
  std::string result = strct->_name + " " + std::string("init_") +  strct->_name + "()\n{\n";
  _indent++;

  const std::string vName("init");
  result += indent() + strct->_name + " " + vName + ";\n";

  std::string rightHand = initializations;
  while(true) {
    std::size_t pos = rightHand.find(";");
    if(pos == std::string::npos)
      break;

    std::string sub = rightHand.substr(0, pos+1);
    // check if we can find an assignment
    std::size_t subPos = sub.find("=");
    if(subPos != std::string::npos) {
      std::string assignment = sub.substr(subPos+1);
      std::string leftHand = sub.substr(0, subPos);
      
      std::size_t lastSpace = leftHand.length() - 1;
      while(lastSpace == leftHand.length()-1) {
	leftHand = leftHand.substr(0, leftHand.length()-1);
	lastSpace = leftHand.find_last_of(" ");
      }

      std::string variableName = leftHand.substr(lastSpace+1);
      std::string codeLine = vName + "." + variableName + " =" + assignment;
      result += indent() + codeLine + "\n";
      
    }
             
    rightHand = rightHand.substr(pos+1);
  }
  

  result += indent() + "return " + vName + ";\n";
  _indent--;
  result += indent() + std::string("}\n\n");
  return result;
}

std::string Transpiler::operateOn(struct Struct * strct)
{
  std::string result = indent();
  result = result + "struct " + strct->_name + "\n";

  std::string variables = traverse(&strct->_block);
  std::string decls = variables;
  std::string declsWithoutAssignment;
  while(true) {
    std::size_t pos = decls.find(";");
    if(pos == std::string::npos)
      break;

    std::string sub = decls.substr(0, pos+1);
    // check if we can find an assignment
    std::size_t subPos = sub.find("=");
    if(subPos != std::string::npos) {
      std::string leftHand = sub.substr(0,subPos);
      while(leftHand.find_last_of(" ") == (leftHand.length()-1))
	leftHand = leftHand.substr(0, leftHand.length()-1);
      
      declsWithoutAssignment = declsWithoutAssignment + leftHand + ";";
      // remove
      
    }
    else {
      declsWithoutAssignment = declsWithoutAssignment + sub;
    }
    
    decls = decls.substr(pos+1);
  }
  

  result += declsWithoutAssignment;
  result = result + indent() + "\n};\n\n";

  result = result + createStructInitializer(strct, variables);

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
    case UnaryType::PreFixPlusPlus:
      result = result + "++";
      break;
    case UnaryType::PreFixMinusMinus:
      result = result + "--";
      break;
    default:
      break;
    }
  
  result = result + traverse(desc->_expression);
  
  // post expression
  switch (desc->_type)
    {
    case UnaryType::Parenthesis:
      result = result + ")";
      break;
    case UnaryType::PostFixPlusPlus:
      result = result + "++";
      break;
    case UnaryType::PostFixMinusMinus:
      result = result + "--";
      break;
    default:
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
  
  std::string realType = mapToGLType(node);
  std::string realVariableName = toCommaSeparatedList(node->_variableNames, true /* map each variable name */);
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

std::string Transpiler::baseOutVariableName() const
{
  switch(_shader->_functionType)
  {
  case FunctionType::Vertex:
    return "frag_in";
    break;
  default:
    return "result";
  }
  
}
