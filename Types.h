#ifndef _TYPES_H_
#define _TYPES_H_

#include <vector>
#include <string>

class Visitor;

struct Node
{
public:
  Node()
  {
    //    printf("Node::Node()\n");
  }
  
  virtual ~Node() {}
  virtual void visit(Visitor * v);
};

struct VariableAttribute : public Node
{
 public:
  VariableAttribute(const std::string & sAttribute, int iAttribute = -1 )
    :_sAttribute(sAttribute)
    ,_iAttribute(iAttribute)
    {
    }

  virtual ~VariableAttribute() {}
  void visit(Visitor *v);

  std::string _sAttribute;
  int _iAttribute;
};

struct Statement : public Node
{
 public:
  virtual ~Statement() {}
  void visit(Visitor * v) override;
};

struct Block : public Node
{
 public:
  virtual ~Block() {}
  void visit(Visitor * v) override;
  std::vector<Node *> _nodes;
};

struct Program : public Block
{
 public:
  Program(Block * src = nullptr) {
    if(src!=nullptr)
      _nodes = src->_nodes;
  }
  
  void visit(Visitor * v) override;
};


struct UsingDeclaration : public Statement
{
 public:
  UsingDeclaration(const std::string & nmspace)
    :_nmspace(nmspace)
  {
  }
  
  virtual ~UsingDeclaration() {}
  void visit(Visitor * v) override;

  std::string _nmspace;
};

struct Struct : public Statement
{
 public:
  Struct(const std::string & name)
    :_name(name)
  {
  }
  
  virtual ~Struct() {}
  void visit(Visitor * v) override;

  std::string _name;
  Block _block;
};

struct BufferDescriptor : public Node
{
public:
  BufferDescriptor(const std::string & type, const std::string & accessor)
    :_type(type)
    ,_accessor(accessor)
  {
  }

  virtual ~BufferDescriptor() {}
  void visit(Visitor * v) override;
  
  std::string _type;
  std::string _accessor;
};


struct Expression : public Statement
{
 public:
  Expression() {}
  virtual ~Expression() {}
  void visit(Visitor * v) override;

};


enum class ConstantType
{
	Int,
	Float,
	Double,
	Half,
	Identifier,
};

struct ConstantExpression : public Expression
{
public:
	ConstantExpression(ConstantType type, int value)
		:_type(type)
	{
		_int = value;
	}

	ConstantExpression(ConstantType type, float value)
		:_type(type)
	{
		switch (type)
		{
		case ConstantType::Float:
			_float = value;
			break;
		case ConstantType::Half:
			_half = value;
			break;
		}
	}

	ConstantExpression(ConstantType type, double value)
		:_type(type)
	{
		_double = value;
	}

	ConstantExpression(ConstantType type, const std::string & value)
		:_type(type)
	{
		_identifier = value;
	}

	ConstantExpression() {}
	virtual ~ConstantExpression() {}
	void visit(Visitor * v) override;

	ConstantType _type;

	union 
	{
		int _int;
		float _float;
		double _double;
		float _half;
		std::string _identifier;
	};

};

enum class UnaryType
{
	Minus,
	Parenthesis,
};

struct UnaryExpression : public Expression
{
public:
	UnaryExpression(UnaryType type, Expression * expression)
		:_type(type)
		,_expression(expression)
	{

	}

	virtual ~UnaryExpression() {}
	void visit(Visitor * v) override;

	UnaryType _type;
	Expression * _expression;
};

enum class BinaryOperator
{
  Plus,
  Minus,
  Multiply,
  Divide,
  Dot
};

struct BinaryExpression : public Statement
{
 public:
  BinaryExpression(Expression * left, BinaryOperator op, Expression * right)
    :_left(left)
    ,_right(right)
    ,_op(op)
  {
  }

  virtual ~BinaryExpression() {}
  void visit(Visitor * v) override;

  Expression * _left;
  Expression * _right;
  BinaryOperator _op;
};

enum class ReservedToken
{
  None,
  Star,
  Ampersand,
};


struct VariableDeclaration : public Statement
{
 public:
  enum class Qualifier
  {
    None,
    Constant,
  };
    
 VariableDeclaration(Qualifier qualifier,
		     const std::string & type = std::string("") ,
		     BufferDescriptor * bufferDescriptor = nullptr,
		     const ReservedToken reservedToken = ReservedToken::None,
		     const std::string & variableName = std::string(""),
		     VariableAttribute * attribute = nullptr)
    :_type(type)
    ,_variableName(variableName)
    ,_attribute(attribute)
    ,_qualifier(qualifier)
    ,_reservedToken(reservedToken)
    ,_bufferDescriptor(bufferDescriptor)
  {
  }
  
  void visit(Visitor * v) override;

  std::string _type;
  std::string _variableName;
  VariableAttribute * _attribute;
  Qualifier _qualifier;
  ReservedToken _reservedToken;
  BufferDescriptor * _bufferDescriptor;
};

struct VariableList : public Node
{
 public:
  virtual ~VariableList() {}
  void visit(Visitor * v) override;
  
  std::vector<VariableDeclaration *> _variableDeclarations;
  
};

struct FunctionDeclaration : public Statement
{
 public:
  FunctionDeclaration(const std::string & functionType,
		      const std::string & returnType,
		      const std::string & name,
		      VariableList * variables,
		      Block * block)
    :_functionType(functionType)
    ,_returnType(returnType)
    ,_name(name)
    ,_variables(variables)
    ,_block(block)
  {
  }
		      
  virtual ~FunctionDeclaration() {}
  void visit(Visitor * v) override;

  std::string _functionType;
  std::string _returnType;
  std::string _name;
  VariableList * _variables;
  Block * _block;

};

struct FunctionCallArgumentList : public Expression
{
public:
	FunctionCallArgumentList()
	{

	}

	virtual ~FunctionCallArgumentList();
	void visit(Visitor * v) override;

	std::vector<Expression *> _expressions;
};

struct FunctionCall : public Expression
{
	FunctionCall(const std::string & name, FunctionCallArgumentList * arguments)
	{

	}

	virtual~FunctionCall();
	void visit(Visitor * v) override;

	std::string _name;
	FunctionCallArgumentList * arguments;


};

extern Program * _root;

#endif


