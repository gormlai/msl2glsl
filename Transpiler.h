#ifndef _TRANSPILER_H_
#define _TRANSPILER_H_

#include "Visitor.h"
#include <string>

class Transpiler : public Visitor
{
public:



	virtual ~Transpiler() {}

	std::string convert(struct Block * program, struct FunctionDeclaration * shader);

	void operateOn(struct AssignStatement * desc) override;
	void operateOn(struct BinaryExpression * desc) override;
	void operateOn(struct Block * block) override;
	void operateOn(struct BufferDescriptor * desc) override;
	void operateOn(struct ConstantExpression * desc) override;
	void operateOn(struct Expression * desc) override;
	void operateOn(struct FunctionCall * node) override;
	void operateOn(struct FunctionCallArgumentList * node) override;
	void operateOn(struct FunctionDeclaration * node) override;
	void operateOn(struct Node * node) override;
	void operateOn(struct Program * program) override;
	void operateOn(struct ReturnStatement * statement) override;
	void operateOn(struct Statement * statement) override;
	void operateOn(struct Struct * strct) override;
	void operateOn(struct UnaryExpression * desc) override;
	void operateOn(struct UsingDeclaration * usingDecl) override;
	void operateOn(struct VariableAttribute * node) override;
	void operateOn(struct VariableDeclaration * node) override;
	void operateOn(struct VariableList * node) override;

private:
	std::string _shaderString;
	int _indent;
	FunctionDeclaration * _shader;
	VariableDeclaration * _inDecl;

	void indent();
	std::string mapIdentifier(const std::string & src) const;
	std::string outputMain();
	std::string outputInOutUniforms();
	void categoriseVariableDeclaration(VariableDeclaration * vDecl);

};

#endif
