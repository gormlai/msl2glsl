#ifndef _TYPES_H_
#define _TYPES_H_

#include <vector>
#include <string>
#include <map>

class Visitor;

enum class NodeType {
  Assignment,
    BinaryExpression,
    Block,
    BufferDescriptor,
    CastExpression,
    CompareExpression,
    ConstantExpression,
    DeclarationSpecifier,
    DeclarationSpecifierList,
    Preprocessor,
    Expression,
    ForLoop,
    FunctionCall,
    FunctionCallArgumentList,
    FunctionDeclaration,
    LabeledStatement,
    SelectionStatement,
    Node,
    Program,
    Qualifier,
    JumpStatement,
    Statement,
    Struct,
    TypeSpecifier,
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
    Kernel,
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
  virtual NodeType getNodeType() const { return NodeType::Node; }
  Node * getParent() { return _parent; }
  virtual std::vector<Node*> getChildren() { return std::vector<Node*>(); }

public:
	Node * _parent;
};

struct VariableAttribute : public Node
{
 public:
  VariableAttribute(const std::string & sAttribute, struct Expression * eAttribute = nullptr )
    :_sAttribute(sAttribute)
    ,_eAttribute(eAttribute)
    {
    }

  virtual ~VariableAttribute() {}
  
  void visit(Visitor *v) override;
  NodeType getNodeType() const override { return NodeType::VariableAttribute; }

  std::string _sAttribute;
  Expression * _eAttribute;
};

struct DeclarationSpecifier : public Node
{
 public:
 DeclarationSpecifier()
   {
   }
  
 virtual ~DeclarationSpecifier() {}
 void visit(Visitor * v) override;
 NodeType getNodeType() const override { return NodeType::DeclarationSpecifier; }  
};

struct DeclarationSpecifierList : public Node
{
 public:
 DeclarationSpecifierList()
   {
   }
  
 virtual ~DeclarationSpecifierList() {}
 void visit(Visitor * v) override;
 NodeType getNodeType() const override { return NodeType::DeclarationSpecifierList; }

 std::vector<DeclarationSpecifier*> _specifiers;
};

enum class ETypeSpecifier
{
  Int,
    UnsignedInt,
    Bool,
    Float,
    Double,
    Half,
    Custom,
};

struct TypeSpecifier : public DeclarationSpecifier
{
 public:
 TypeSpecifier(ETypeSpecifier type, const std::string customTypeName = "")
   :_type(type)
    ,_customTypeName(customTypeName)
    {
    }

  virtual ~TypeSpecifier() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::TypeSpecifier; }  

  ETypeSpecifier _type;
  std::string _customTypeName;
};

struct Statement : public Node
{
 public:
  virtual ~Statement() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::Statement; }
};

struct Block : public Statement
{
 public:
  virtual ~Block() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::Block; }
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
  NodeType getNodeType() const override { return NodeType::Program; }
};

struct Preprocessor : public Statement
{
 public:
  Preprocessor(const std::string & definition)
    :_definition(definition)
  {
  }

  virtual ~Preprocessor() {}

  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::Preprocessor; }

  std::string _definition;  
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
  NodeType getNodeType() const override { return NodeType::UsingDeclaration; }

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
  NodeType getNodeType() const override { return NodeType::Struct; }
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(&_block); return nodes; }

  std::vector<struct VariableDeclaration*> getVariables() const;
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
  NodeType getNodeType() const override { return NodeType::BufferDescriptor; }
  
  std::string _type;
  std::string _accessor;
};


struct Expression : public Statement
{
 public:
  Expression() {}
  virtual ~Expression() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::Expression; }

};


enum class ConstantType
{
  Int,
    Hex,
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
	NodeType getNodeType() const override { return NodeType::ConstantExpression; }

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
    PreFixPlusPlus,
    PostFixPlusPlus,
    PreFixMinusMinus,
    PostFixMinusMinus,
    Dereference,
    TakeAddressOf,
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
	NodeType getNodeType() const override { return NodeType::UnaryExpression; }

	UnaryType _type;
	Expression * _expression;
	std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_expression); return nodes; }
};

enum class CompareOperator
  {
    LessThan,
      GreaterThan,
      LessThanOrEqualTo,
      GreaterThanOrEqualTo,
      EqualTo,
      NotEqualTo,
  };

struct CompareExpression : public Expression
{
 public:
  CompareExpression(Expression * left, CompareOperator op, Expression * right)
    :_left(left)
    ,_right(right)
    ,_op(op)
  {
	  left->_parent = this;
	  right->_parent = this;
  }

  virtual ~CompareExpression() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::CompareExpression; }
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_left); nodes.push_back(_right);  return nodes; }
  Expression * _left;
  Expression * _right;
  CompareOperator _op;
};


enum class BinaryOperator
{
  Plus,
  Minus,
  Multiply,
  Divide,
    Dot,
    Pointer,
    Cast,
    BinaryOr,
    LogicalOr,
    BinaryAnd,
    LogicalAnd,
    LeftShift,
    RightShift,
    Index,
    XOr,
    Modulo,
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
  NodeType getNodeType() const override { return NodeType::BinaryExpression; }
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

struct CastExpression : public Expression
{
 public:
  CastExpression(const std::string & castTo, Expression * right)
    :_castTo(castTo)
    ,_right(right)
  {
    _right->_parent = this;
  }

  virtual ~CastExpression() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::CastExpression; }

  std::string _castTo;
  Expression * _right;
  
};

enum class QualifierType
{
  Constant,
    Const,
    Constexpr,
    Device,
    Signed,
    Unsigned,
    Static,
};

struct Qualifier : public DeclarationSpecifier
{
 public:
  Qualifier(QualifierType type)
    :_type(type)
  {
  }

  virtual ~Qualifier() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::Qualifier; }
  
  QualifierType _type;
};

struct VariableDeclaration : public Statement
{
 public:
  enum class Qualifier
  {
  };
    
 VariableDeclaration(DeclarationSpecifierList * declarationSpecifiers,
		     BufferDescriptor * bufferDescriptor,
		     const ReservedToken reservedToken,
		     const std::vector<struct VariableNameDeclaration*> & variableNames,
		     VariableAttribute * attribute = nullptr)
   :_declarationSpecifiers(declarationSpecifiers)
    ,_variableNames(variableNames)
    ,_attribute(attribute)
    ,_reservedToken(reservedToken)
    ,_bufferDescriptor(bufferDescriptor)
  {
	 if (attribute != nullptr)
	   attribute->_parent = this;

	 if (bufferDescriptor != nullptr)
	   bufferDescriptor->_parent = this;

	 if (declarationSpecifiers != nullptr)
	   declarationSpecifiers->_parent = this;
  }
  
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::VariableDeclaration; }

  DeclarationSpecifierList * _declarationSpecifiers;
  std::vector<VariableNameDeclaration*> _variableNames;
  VariableAttribute * _attribute;
  ReservedToken _reservedToken;
  BufferDescriptor * _bufferDescriptor;
};

struct VariableList : public Node
{
 public:
  virtual ~VariableList() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::VariableList; }
  
  std::vector<VariableDeclaration *> _variableDeclarations;
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; for(auto v : _variableDeclarations) nodes.push_back(v); return nodes;  }  
};

enum class SelectionStatementType
{
  If,
    Else,
    ElseIf,
    Switch
};

struct SelectionStatement : public Statement
{
 public:
  SelectionStatement(SelectionStatementType ifType, Expression * conditional, Statement * statement)
    :_ifType(ifType)
    ,_conditional(conditional)
    ,_statement(statement)
    {
      if(_conditional != nullptr)
	_conditional->_parent = this;

      if(_statement != nullptr)
	_statement->_parent = this;
    }

  virtual ~SelectionStatement() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::SelectionStatement; }

  SelectionStatementType _ifType;
  Expression * _conditional;
  Statement * _statement;
};

enum class LabeledStatementType
{
  Case,
    Default,
};

struct LabeledStatement : public Statement
{
  LabeledStatement(LabeledStatementType type, Expression * label, Statement * statement)
    :_type(type)
    ,_label(label)
    ,_statement(statement)
  {
  }

  virtual ~LabeledStatement() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::LabeledStatement; }  

  LabeledStatementType _type;
  Expression *  _label;
  Statement * _statement;
};

struct ForLoop : public Statement
{
 public:
 ForLoop(Node * variableDeclarations, Node * conditionals, Node * increment, Node * loop)
    :_variableDeclarations(variableDeclarations)
    ,_conditionals(conditionals)
    ,_increment(increment)
    ,_loop(loop)
    {
      if(_variableDeclarations!=nullptr)
	_variableDeclarations->_parent = this;
      
      if(_conditionals!=nullptr)
	_conditionals->_parent = this;
      
      if(_increment!=nullptr)
	_increment->_parent = this;

      if(_loop!=nullptr)
	_loop->_parent = this;
    }

  virtual ~ForLoop() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::ForLoop; }
  
  Node * _variableDeclarations;
  Node * _conditionals;
  Node * _increment;
  Node * _loop;
};

struct FunctionDeclaration : public Statement
{
 public:
 FunctionDeclaration(const FunctionType functionType,
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
    variables->_parent = this;
    block->_parent = this;
  }
		      
  virtual ~FunctionDeclaration() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::FunctionDeclaration; }
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
	NodeType getNodeType() const override { return NodeType::FunctionCallArgumentList; }

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
	NodeType getNodeType() const override { return NodeType::FunctionCall; }

	std::string _name;
	FunctionCallArgumentList * _arguments;
	std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_arguments); return nodes; }

};

enum class AssignOperator {
  Equal,
    Plus,
    Minus,
    Divide,
    Multiply,
    And,
    XOr,
    Or,
    Modulo,
};


struct Assignment : public Expression
{
 Assignment(Node * left, AssignOperator op,  Node * right)
   :_left(left)
    ,_operator(op)
    ,_right(right)
  {
	 left->_parent = this;
	 right->_parent = this;
  }

  virtual ~Assignment() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::Assignment; }
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_left); nodes.push_back(_right); return nodes; }
  
  Node * _left;
  AssignOperator _operator;
  Node * _right;
      
};

enum class JumpStatementType
{
  Return,
    Break,
    Continue,
};

struct JumpStatement : public Statement
{
 JumpStatement(JumpStatementType type, Expression * e)
   :_type(type)
   ,_expression(e)
  {
	  if(_expression!=nullptr)
	  _expression->_parent = this;
  }

  virtual ~JumpStatement() {}
  void visit(Visitor * v) override;
  NodeType getNodeType() const override { return NodeType::JumpStatement; }

  JumpStatementType _type;
  Expression * _expression;
  std::vector<Node*> getChildren() override { std::vector<Node*> nodes; nodes.push_back(_expression); return nodes; }      
};

struct SelectExpression : public Expression
{
  SelectExpression(Node * left, Node * middle, Node * right)
    :_left(left)
    ,_middle(middle)
    ,_right(right)
  {
    if(_left!=nullptr)
      _left->_parent = this;

    if(_middle!=nullptr)
      _middle->_parent = this;
    
    if(_right!=nullptr)
      _right->_parent = this;
    
  }

  virtual ~SelectExpression() {}

  Node * _left;
  Node * _middle;
  Node * _right;
};

struct VariableNameDeclaration : public Node
{
  VariableNameDeclaration(const std::string & variableName, Expression * expressionInBrackets)
    :_variableName(variableName)
    ,_expressionInBrackets(expressionInBrackets) {
  }
  
  std::string _variableName;
  Expression * _expressionInBrackets;
};



extern Program * _root;

#endif


