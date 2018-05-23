#ifndef _TYPES_H_
#define _TYPES_H_

#include <vector>
#include <string>

class Visitor;

enum class NodeType {
  AssignStatement,
    BinaryExpression,
    Block,
    BufferDescriptor,
    ConstantExpression,
    Expression,
    FunctionCall,
    FunctionCallArgumentList,
    FunctionDeclaration,
    Node,
    Program,
    ReturnStatement,
    Statement,
    Struct,
    UnaryExpression,
    UsingDeclaration,
    VariableAttribute,
    VariableDeclaration,
    VariableList,
    };

enum class FunctionType {
  Utility,
    Vertex,
    Fragment,
    Compute,
};


struct Node
{
public:
  Node()
	  :_parent(nullptr)
  {
    //    printf("Node::Node()\n");
  }
  
  virtual ~Node() {}
  virtual void visit(Visitor * v);
  virtual NodeType getNodeType() { return NodeType::Node; }
  Node * getParent() { return _parent; }
  virtual std::vector<Node*> getChildren() { return std::vector<Node*>(); }

public:
	Node * _parent;
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
  NodeType getNodeType() override { return NodeType::VariableAttribute; }

  std::string _sAttribute;
  int _iAttribute;
};

struct Statement : public Node
{
 public:
  virtual ~Statement() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() override { return NodeType::Statement; }
};

struct Block : public Node
{
 public:
  virtual ~Block() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() override { return NodeType::Block; }
  std::vector<Node*> getChildren() override { return _nodes; }

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
  NodeType getNodeType() override { return NodeType::Program; }
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
  NodeType getNodeType() override { return NodeType::UsingDeclaration; }

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
  NodeType getNodeType() override { return NodeType::Struct; }
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(&_block); return nodes; }
  
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
  NodeType getNodeType() override { return NodeType::BufferDescriptor; }
  
  std::string _type;
  std::string _accessor;
};


struct Expression : public Statement
{
 public:
  Expression() {}
  virtual ~Expression() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() override { return NodeType::Expression; }

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
		default: // we just need to add this to silence the compiler warnings
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
	NodeType getNodeType() override { return NodeType::ConstantExpression; }

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
		expression->_parent = this;
	}

	virtual ~UnaryExpression() {}
	void visit(Visitor * v) override;
	NodeType getNodeType() override { return NodeType::UnaryExpression; }

	UnaryType _type;
	Expression * _expression;
	std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_expression); return nodes; }
};

enum class BinaryOperator
{
  Plus,
  Minus,
  Multiply,
  Divide,
  Dot
};

struct BinaryExpression : public Expression
{
 public:
  BinaryExpression(Expression * left, BinaryOperator op, Expression * right)
    :_left(left)
    ,_right(right)
    ,_op(op)
  {
	  left->_parent = this;
	  right->_parent = this;
  }

  virtual ~BinaryExpression() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() override { return NodeType::BinaryExpression; }
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_left); nodes.push_back(_right);  return nodes; }
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
	 if (attribute != nullptr)
		 attribute->_parent = this;

	 if (bufferDescriptor != nullptr)
		 bufferDescriptor->_parent = this;
  }
  
  void visit(Visitor * v) override;
  NodeType getNodeType() override { return NodeType::VariableDeclaration; }
  
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
  NodeType getNodeType() override { return NodeType::VariableList; }
  
  std::vector<VariableDeclaration *> _variableDeclarations;
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; for(auto v : _variableDeclarations) nodes.push_back(v); return nodes;  }  
};

struct FunctionDeclaration : public Statement
{
 public:
 FunctionDeclaration(const std::string & functionType,
		      const std::string & returnType,
		      const std::string & name,
		      VariableList * variables,
		      Block * block)
   :_returnType(returnType)
    ,_name(name)
    ,_variables(variables)
    ,_block(block)
  {
    if(functionType == "vertex")
      _functionType = FunctionType::Vertex;
    else if(functionType == "fragment")
      _functionType = FunctionType::Fragment;
    else
      _functionType = FunctionType::Utility;

	variables->_parent = this;
	block->_parent = this;

  }
		      
  virtual ~FunctionDeclaration() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() override { return NodeType::FunctionDeclaration; }
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_block); return nodes; }  

  FunctionType _functionType;
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

	virtual ~FunctionCallArgumentList() {}
	void visit(Visitor * v) override;
	NodeType getNodeType() override { return NodeType::FunctionCallArgumentList; }

	std::vector<Expression *> _expressions;
	std::vector<Node*> getChildren() override { std::vector<Node*> nodes; for(auto v : _expressions) nodes.push_back(v); return nodes; }
};

struct FunctionCall : public Expression
{
	FunctionCall(const std::string & name, FunctionCallArgumentList * arguments)
	  :_name(name)
    ,_arguments(arguments)
	{
		if (arguments != nullptr)
			arguments->_parent = this;

	}

	virtual ~FunctionCall() {}
	void visit(Visitor * v) override;
	NodeType getNodeType() override { return NodeType::FunctionCall; }

	std::string _name;
	FunctionCallArgumentList * _arguments;
	std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_arguments); return nodes; }

};

struct AssignStatement : public Statement
{
 AssignStatement(Node * left, Node * right)
   :_left(left)
    ,_right(right)
  {
	 left->_parent = this;
	 right->_parent = this;
  }

  virtual ~AssignStatement() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() override { return NodeType::AssignStatement; }
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_left); nodes.push_back(_right); return nodes; }	
  Node * _left;
  Node * _right;
      
};


struct ReturnStatement : public Statement
{
  ReturnStatement(Expression * e)
    :_expression(e)
  {
	  if(_expression!=nullptr)
	  _expression->_parent = this;
  }

  virtual ~ReturnStatement() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() override { return NodeType::ReturnStatement; }
	
  Expression * _expression;
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_expression); return nodes; }      
};

extern Program * _root;

#endif


