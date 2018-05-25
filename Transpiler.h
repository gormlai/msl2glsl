#ifndef _TRANSPILER_H_
#define _TRANSPILER_H_

#include <string>

class Transpiler
{
public:
	virtual ~Transpiler() {}

	std::string convert(struct Block * program, struct FunctionDeclaration * shader);

	std::string operateOn(struct AssignStatement * desc);
	std::string operateOn(struct BinaryExpression * desc);
	std::string operateOn(struct Block * block);
	std::string operateOn(struct BufferDescriptor * desc);
	std::string operateOn(struct ConstantExpression * desc);
	std::string operateOn(struct Expression * desc);
	std::string operateOn(struct FunctionCall * node);
	std::string operateOn(struct FunctionCallArgumentList * node);
	std::string operateOn(struct FunctionDeclaration * node);
	std::string operateOn(struct Node * node);
	std::string operateOn(struct Program * program);
	std::string operateOn(struct ReturnStatement * statement);
	std::string operateOn(struct Statement * statement);
	std::string operateOn(struct Struct * strct);
	std::string operateOn(struct UnaryExpression * desc);
	std::string operateOn(struct UsingDeclaration * usingDecl);
	std::string operateOn(struct VariableAttribute * node);
	std::string operateOn(struct VariableDeclaration * node);
	std::string operateOn(struct VariableList * node);

	std::string traverse(struct Node * node);

private:
	int _indent;
	FunctionDeclaration * _shader;
	VariableDeclaration * _inDecl; // what are the in variables?
	std::map<std::string,Struct*> _topLevelStructs; // for mapping in, out and uniform variables

	std::string indent();
	std::string mapIdentifier(const std::string & src) const;
	std::string outputMain();
	std::string outputInOutUniforms();
	void categoriseVariableDeclaration(VariableDeclaration * vDecl);
	bool isSimpleGLType(const std::string & glType) const;

	

};

#endif
