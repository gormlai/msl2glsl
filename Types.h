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

struct Declaration : public Node
{
 public:
  virtual ~Declaration() {}
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

struct UsingDeclaration : public Declaration
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

struct Struct : public Declaration
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

struct Type : public Node
{
};

struct UChar : public Type
{
  void visit(Visitor * v) override;
};

struct UChar2 : public Type
{
  void visit(Visitor * v) override;
};

struct UChar3 : public Type
{
  void visit(Visitor * v) override;
};

struct UChar4 : public Type
{
  void visit(Visitor * v) override;
};

struct Float : public Type
{
  void visit(Visitor * v) override;
};

struct Float2 : public Type
{
  void visit(Visitor * v) override;
};

struct Float3 : public Type
{
  void visit(Visitor * v) override;
};

struct Float4 : public Type
{
  void visit(Visitor * v) override;
};

struct VariableDeclaration : public Declaration
{
 public:
 VariableDeclaration(Type * type = nullptr, const std::string & variableName = std::string(""), VariableAttribute * attribute = nullptr)
    :_type(type)
      ,_variableName(variableName)
    ,_attribute(attribute)
  {
  }
  
  void visit(Visitor * v) override;

  Type * _type;
  std::string _variableName;
  VariableAttribute * _attribute;
};

extern Program * _root;

#endif


