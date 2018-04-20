#ifndef _TYPES_H_
#define _TYPES_H_

#include <vector>
#include <string>

template<typename T, int SIZE> struct Array
{
public:
  T data[SIZE];
};

typedef Array<float,2> float2;
typedef Array<float,3> float3;
typedef Array<float,4> float4;
typedef Array<unsigned char,2> uchar2;
typedef Array<unsigned char,3> uchar3;
typedef Array<unsigned char,4> uchar4;

class Visitor;

struct Node
{
public:
  virtual ~Node() {}
  virtual void visit(Visitor * v);
};

struct Declaration : public Node
{
 public:
  virtual ~Declaration() {}
  void visit(Visitor * v) override;
};

struct DeclarationList : public Node
{
 public:
  virtual ~DeclarationList() {}
  void visit(Visitor * v) override;
  std::vector<Node *> _nodes;
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

struct Struct : Node
{
 public:
  virtual ~Struct() {}
  void visit(Visitor * v) override;
};

struct Program
{
public:
  Program(DeclarationList * list)
    :_decls(list)
  {
    
  }

  virtual ~Program() {}
  void visit(Visitor * v);
  
  DeclarationList * _decls;
};

extern Program * _root;

#endif


