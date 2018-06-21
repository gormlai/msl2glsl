#include "PrettyPrinter.h"
#include "Types.h"
#include <iostream>

void PrettyPrinter::arrayNumToString(Expression * e) {
  if(e == nullptr)
    return;
  
  _result = _result + std::string("[");
  if(e != nullptr)
    e->visit(this);
  _result = _result + std::string("]");
}
  
void PrettyPrinter::toCommaSeparatedList(const std::vector<VariableNameDeclaration *> & input)
{
  for(unsigned int i=0 ; i < (unsigned int)input.size() ; i++) {    
    VariableNameDeclaration * vDecl = input[i];
    _result = _result + vDecl->_variableName;
    arrayNumToString(vDecl->_expressionInBrackets);
    if(i != input.size()-1)
      _result = _result + ", ";
  }
}


void PrettyPrinter::indent()
{
	for (int i = 0; i < _indent; i++)
		_result = _result + "  ";
}

const std::string PrettyPrinter::print(struct Block * block)
{

	_result = std::string("\n");
	_indent = 0;

	if (block == nullptr) {
		printf("PrettyPrinter - program is nullptr\n");
		return _result;
	}

	block->visit(this);

	return _result;
}




void PrettyPrinter::operateOn(struct Assignment * desc)
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
		"->",
		")",
		"|",
		"||",
		"&",
		"&&",
		"<<",
		">>",
		"[",
		"^",
		"%",
	};

	if(desc->_op == BinaryOperator::Cast)
	  _result = _result + "(";

	desc->_left->visit(this);
	_result = _result + ops[(int)desc->_op];
	desc->_right->visit(this);

	//	if(desc->_op != BinaryOperator::Dot)
	//  _result = _result + "]";

}

void PrettyPrinter::operateOn(struct Block * block)
{
	indent();
	_result = _result + "{\n";

	_indent++;

	for (auto node : block->_nodes)
	{
		if (node == nullptr)
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

void PrettyPrinter::operateOn(struct CastExpression * desc)
{
  _result = _result + "(" + desc->_castTo + "):";
  desc->_right->visit(this);
}

void PrettyPrinter::operateOn(struct CompareExpression * exp)
{
  const char * opMapping [] = {
    "==",
    ">"
    ">="
    "<",
    "<=",
    "!=",
  };

  exp->_left->visit(this);
  _result = _result + " " + opMapping[(int)exp->_op] + " ";
  exp->_right->visit(this);
}

void PrettyPrinter::operateOn(struct ConstantExpression * desc)
{
	switch (desc->_type)
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
	case ConstantType::Hex:
		_result = _result + desc->_identifier;
		break;
	case ConstantType::Identifier:
		_result = _result + desc->_identifier;
		break;
	}

}

void PrettyPrinter::operateOn(struct DeclarationSpecifier * desc)
{
  std::cerr << "DeclarationSpecifier should have been inherited" << std::endl;
}

void PrettyPrinter::operateOn(struct DeclarationSpecifierList * list)
{
  for(DeclarationSpecifier * spec : list->_specifiers)
    spec->visit(this);
}

void PrettyPrinter::operateOn(struct Expression * desc)
{
  std::cerr << "Expression should have been inherited" << std::endl;
}

void PrettyPrinter::operateOn(struct ForLoop * node)
{
  _result = _result + "for (";

  if(node->_variableDeclarations != nullptr)
    node->_variableDeclarations->visit(this);

  _result = _result + " ; ";
  
  if(node->_conditionals != nullptr)
    node->_conditionals->visit(this);

  _result = _result + " ; ";
  
  if(node->_increment != nullptr)
    node->_increment->visit(this);

  _result = _result + ")";
  _result = _result + "\n";
  
  indent();  
  
  if(node->_loop != nullptr)
    node->_loop->visit(this);
  
}

void PrettyPrinter::operateOn(struct FunctionCall * node)
{
	_result = _result + node->_name;
	_result = _result + "(";

	if (node->_arguments != nullptr)
		node->_arguments->visit(this);

	_result = _result + ")";
}

void PrettyPrinter::operateOn(struct FunctionCallArgumentList * node)
{
	const unsigned int size = (unsigned int)node->_expressions.size();
	for (unsigned int i = 0; i < size; i++) {

		Expression * e = node->_expressions[i];
		e->visit(this);

		if (i != size - 1)
			_result = _result + ",";

	}
}

void PrettyPrinter::operateOn(struct FunctionDeclaration * node)
{
	indent();

	std::string functionType = "";
	switch (node->_functionType)
	{
	case FunctionType::Vertex:
		functionType = "vertex";
		break;
	case FunctionType::Fragment:
		functionType = "fragment";
		break;
	default:
		break;
	}

	_result = _result + functionType + " ";

	if(node->_declarationSpecifiers != nullptr)
	  node->_declarationSpecifiers->visit(this);

	_result = _result + " " + node->_name + "(";

	if (node->_variables != nullptr)
		node->_variables->visit(this);

	_result = _result + ")\n";

	if (node->_block != nullptr)
		node->_block->visit(this);

	_result = _result + "\n";
}

void PrettyPrinter::operateOn(struct JumpStatement * statement)
{
	indent();
	_result = _result + "return ";
	statement->_expression->visit(this);
	_result = _result + ";\n";
}

void PrettyPrinter::operateOn(struct LabeledStatement * statement)
{
  indent();

  switch(statement->_type)
    {
    case LabeledStatementType::Case:
    _result = _result + "case ";
    statement->_label->visit(this);
    _result = _result + ":\n";
    indent();
    statement->_statement->visit(this);    
    break;
    case LabeledStatementType::Default:
    _result = _result + "default:\n";
    indent();
    statement->_statement->visit(this);    
    break;    
  }

  indent();
  _result = _result + "break;\n";
  
}

void PrettyPrinter::operateOn(struct Preprocessor * def)
{
  _result = _result + def->_definition;
}


void PrettyPrinter::operateOn(struct Program * program)
{
	for (auto node : program->_nodes)
		node->visit(this);
}

void PrettyPrinter::operateOn(struct Qualifier * qualifier)
{
  switch (qualifier->_type)
    {
    case QualifierType::Constant:
      _result = _result + "constant ";
      break;
    default:
      break;
    }
}

void PrettyPrinter::operateOn(struct SelectionStatement * statement)
{
  switch(statement->_ifType)
    {
    case SelectionStatementType::If:
      _result = _result + "if( ";
      statement->_conditional->visit(this);
      _result = _result + " )\n";
      break;
    case SelectionStatementType::Else:
      _result = _result + "else\n";
      break;
    case SelectionStatementType::ElseIf:
      _result = _result + "else if( ";
      statement->_conditional->visit(this);
      _result = _result + " )\n";
      break;
    case SelectionStatementType::Switch:
      _result = _result + "switch( ";
      statement->_conditional->visit(this);
      _result = _result + " )\n";
      break;
    }  
  statement->_statement->visit(this);
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

void PrettyPrinter::operateOn(struct TypeSpecifier * spec)
{
  switch(spec->_type)
    {
    case ETypeSpecifier::Int:
      _result = _result + "int";
      break;
    case ETypeSpecifier::UnsignedInt:
      _result = _result + "unsigned int";
      break;
    case ETypeSpecifier::Bool:
      _result = _result + "bool";
      break;
    case ETypeSpecifier::Float:
      _result = _result + "float";
      break;
    case ETypeSpecifier::Double:
      _result = _result + "double";
      break;
    case ETypeSpecifier::Half:
      _result = _result + "half";
      break;
    case ETypeSpecifier::Custom:
      _result = _result + spec->_customTypeName;
      break;
    }
}

void PrettyPrinter::operateOn(struct UnaryExpression * desc)
{
	// pre expression
	switch (desc->_type)
	{
	case UnaryType::Minus:
	  _result = _result + "-";
	  break;
	case UnaryType::Parenthesis:
	  _result = _result + "(";
	  break;
	case UnaryType::PreFixMinusMinus:
	  _result = _result + "--";
	  break;
	case UnaryType::PreFixPlusPlus:
	  _result = _result + "++";
	  break;
	default:
	  break;
	}

	desc->_expression->visit(this);

	// post expression
	switch (desc->_type)
	{
	case UnaryType::Parenthesis:
	  _result = _result + ")";
	  break;
	case UnaryType::PostFixMinusMinus:
	  _result = _result + "--";
	  break;
	case UnaryType::PostFixPlusPlus:
	  _result = _result + "++";
	  break;
	default:
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

	auto innerAttribute = [&,this](const std::string & sAttribute) {
	  _result = _result + sAttribute;
	  _result = _result + "(";
	  attribute->_eAttribute->visit(this);
	  _result = _result + ")";
	};
	
	if (attribute->_sAttribute == std::string("attribute"))
	  innerAttribute(attribute->_sAttribute);	
	else if (attribute->_sAttribute == std::string("position"))
	  _result = _result + "position";
	else if (attribute->_sAttribute == std::string("stage_in"))
	  _result = _result + "stage_in";
	else if (attribute->_sAttribute == std::string("buffer"))
	  innerAttribute(attribute->_sAttribute);	
	else if (attribute->_sAttribute == std::string("texture"))
	  innerAttribute(attribute->_sAttribute);	
	else if (attribute->_sAttribute == std::string("sampler"))
	  innerAttribute(attribute->_sAttribute);	

	_result = _result + "]]";
}



void PrettyPrinter::operateOn(struct VariableDeclaration * node)
{
	indent();

	if(node->_declarationSpecifiers != nullptr)
	  node->_declarationSpecifiers->visit(this);

	if (node->_bufferDescriptor != nullptr)
		node->_bufferDescriptor->visit(this);

	std::string sToken = "";
	switch (node->_reservedToken)
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

	_result = _result + sToken + " ";
	toCommaSeparatedList(node->_variableNames);

	if (node->_attribute != nullptr)
		node->_attribute->visit(this);


}

void PrettyPrinter::operateOn(struct VariableList * node)
{
	const int count = (int)node->_variableDeclarations.size();
	for (int i = 0; i < count; i++) {
		node->_variableDeclarations[i]->visit(this);

		if (i != count - 1)
			_result = _result + ", ";
	}
}

