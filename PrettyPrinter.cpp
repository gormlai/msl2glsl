#include "PrettyPrinter.h"
#include "Types.h"

void PrettyPrinter::indent()
{
  for(int i =0 ; i < _indent ; i++)
    _result = _result + "  ";
}

const std::string PrettyPrinter::print(struct Block * block)
{
  
  _result = std::string("\n");
  _indent = 0;
  
  if(block == nullptr) {
    printf("PrettyPrinter - program is nullptr\n");
    return _result;
  }
  
  block->visit(this);
  
  return _result;
}

void PrettyPrinter::operateOn(struct AssignStatement * desc)
{
  indent();
  desc->_left->visit(this);
  _result = _result + " = ";
  desc->_right->visit(this);
}


void PrettyPrinter::operateOn(struct BinaryExpression * desc)
{
  const static std::string ops[] =
    {
      "+",
      "-",
      "*",
      "/",
      ".",
    };

  //if(desc->_op != BinaryOperator::Dot)
    //    _result = _result + "(";
  
  desc->_left->visit(this);
  _result = _result + ops[(int)desc->_op];
  desc->_right->visit(this);
  
  //  if(desc->_op != BinaryOperator::Dot)
  //  _result = _result + ")";
  
}

void PrettyPrinter::operateOn(struct Block * block)
{  
  indent();
  _result = _result + "{\n";

  _indent++;

  for(auto node : block->_nodes)
    {
      if(node == nullptr)
	continue;
      
      node->visit(this);
      _result = _result + ";\n";
  }

  _indent--;
  
  indent();
  _result = _result + "}";
}

void PrettyPrinter::operateOn(struct BufferDescriptor * desc)
{
  _result = _result + "<" + desc->_type + ",access::" + desc->_accessor + ">";
}

void PrettyPrinter::operateOn(struct ConstantExpression * desc)
{
  switch(desc->_type)
    {
    case ConstantType::Int:
      _result = _result + std::to_string(desc->_int);
      break;
    case ConstantType::Float:
      _result = _result + std::to_string(desc->_float);
      break;
    case ConstantType::Double:
      _result = _result + std::to_string(desc->_double);
      break;
    case ConstantType::Half:
      _result = _result + std::to_string(desc->_half);
      break;
    case ConstantType::Identifier:
      _result = _result + desc->_identifier;
      break;
    }

}

void PrettyPrinter::operateOn(struct Expression * desc)
{
  // Expression is a Base class. Don't do anything here
}


void PrettyPrinter::operateOn(struct FunctionCall * node)
{
  _result = _result + node->_name;
  _result = _result + "(";

  if(node->_arguments != nullptr)
    node->_arguments->visit(this);

  _result = _result + ")";  
}

void PrettyPrinter::operateOn(struct FunctionCallArgumentList * node)
{
  const unsigned int size = (unsigned int)node->_expressions.size();
  for(unsigned int i=0 ; i < size ; i++ ) {

    Expression * e = node->_expressions[i];
    e->visit(this);

    if(i != size-1)
      _result = _result + ",";
    
  }
}

void PrettyPrinter::operateOn(struct FunctionDeclaration * node)
{
  indent();

  std::string functionType = "";
  switch(node->_functionType)
    {
    case FunctionType::Vertex:
      functionType = "vertex";
      break;
    default:
      break;
    }
  
      _result = _result + functionType + " ";

  if(!node->_returnType.empty())
    _result = _result + node->_returnType + " ";
  
  _result = _result + node->_name + "(";

  if(node->_variables!=nullptr)
    node->_variables->visit(this);
  
  _result = _result + ")\n";

  if(node->_block!=nullptr)
    node->_block->visit(this);

  _result = _result + "\n";
}

void PrettyPrinter::operateOn(struct Node * node)
{
  // Node is a base class. Implementation should be in base classes
}

void PrettyPrinter::operateOn(struct Program * program)
{
  for(auto node : program->_nodes)
    node->visit(this);
}

void PrettyPrinter::operateOn(struct ReturnStatement * statement)
{
  indent();
  _result = _result + "return ";
  statement->_expression->visit(this);
  _result = _result + ";\n"; 
}

void PrettyPrinter::operateOn(struct Statement * statement)
{
  // Statement is a base class. Implementation should be in base classes  
}

void PrettyPrinter::operateOn(struct Struct * strct)
{
  indent();
  _result = _result + "struct " + strct->_name + "\n";

  strct->_block.visit(this);
  
  indent();
  _result = _result + ";\n";
  
}

void PrettyPrinter::operateOn(struct UnaryExpression * desc)
{
  // pre expression
  switch(desc->_type)
    {
    case UnaryType::Minus:
      _result = _result + "-";
      break;
    case UnaryType::Parenthesis:
      _result = _result + "(";
      break;
    }

  desc->_expression->visit(this);

  // post expression
  switch(desc->_type)
    {
    case UnaryType::Minus:
      _result = _result + "-";
      break;
    case UnaryType::Parenthesis:
      _result = _result + ")";
      break;
    }
  
}


void PrettyPrinter::operateOn(struct UsingDeclaration * usingDecl)
{
  indent();
  _result = _result + "using namespace " + usingDecl->_nmspace + ";\n";
}

void PrettyPrinter::operateOn(struct VariableAttribute * attribute)
{
  _result = _result + " [[";
  
  if(attribute->_sAttribute == std::string("attribute"))
    _result = _result + "attribute(" + std::to_string(attribute->_iAttribute) + ")";
  else if(attribute->_sAttribute == std::string("position"))
    _result = _result + "position";
  else if(attribute->_sAttribute == std::string("stage_in"))
    _result = _result + "stage_in";
  else if(attribute->_sAttribute == std::string("buffer"))
    _result = _result + "buffer(" + std::to_string(attribute->_iAttribute) + ")";
  else if(attribute->_sAttribute == std::string("texture"))
    _result = _result + "texture(" + std::to_string(attribute->_iAttribute) + ")";
  else if(attribute->_sAttribute == std::string("sampler"))
    _result = _result + "sampler(" + std::to_string(attribute->_iAttribute) + ")";

  _result = _result + "]]";  
}

void PrettyPrinter::operateOn(struct VariableDeclaration * node)
{
  indent();

  switch(node->_qualifier)
    {
    case VariableDeclaration::Qualifier::Constant:
      _result = _result + "constant ";
      break;
    default:
      break;
    }
  
  _result =  _result + node->_type;

  if(node->_bufferDescriptor!=nullptr)
    node->_bufferDescriptor->visit(this);

  std::string sToken = "";
  switch(node->_reservedToken)
    {
    case ReservedToken::Star:
      sToken = std::string(" *");
      break;
    case ReservedToken::Ampersand:
      sToken = std::string(" &");
      break;
    case ReservedToken::None:
    default:
      break;
    }

  _result = _result + sToken + " " + node->_variableName;

  if(node->_attribute != nullptr)
    node->_attribute->visit(this);

  
}

void PrettyPrinter::operateOn(struct VariableList * node)
{
  const int count =  (int)node->_variableDeclarations.size(); 
  for(int i=0 ; i < count ; i++) {
    node->_variableDeclarations[i]->visit(this);
    
    if(i!=count-1)
      _result = _result + ", ";
  }
}

