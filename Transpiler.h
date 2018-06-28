#ifndef _TRANSPILER_H_
#define _TRANSPILER_H_

#include <string>

enum class TranspilerState {
  Init = 0,
    OutputGlobals = 1,
    OutputMain = 2,
    OutputRestOfProgram = 3 ,
    CleaningUp = 4,
    };

class Transpiler
{
public:
	virtual ~Transpiler() {}

	std::string convert(struct Block * program, struct FunctionDeclaration * shader);

	std::string operateOn(struct Assignment * desc);
	std::string operateOn(struct BinaryExpression * desc);
	std::string operateOn(struct Block * block);
	std::string operateOn(struct BufferDescriptor * desc);
	std::string operateOn(struct CastExpression * desc);
	std::string operateOn(struct ConstantExpression * desc);
	std::string operateOn(struct DeclarationSpecifier * desc);
	std::string operateOn(struct DeclarationSpecifierList * descList);
	std::string operateOn(struct Expression * desc);
	std::string operateOn(struct FunctionCall * node);
	std::string operateOn(struct FunctionCallArgumentList * node);
	std::string operateOn(struct FunctionDeclaration * node);
	std::string operateOn(struct JumpStatement * statement);
	std::string operateOn(struct LabeledStatement * statement);
	std::string operateOn(struct Node * node);
	std::string operateOn(struct Program * program);
	std::string operateOn(struct Qualifier * qualifier);
	std::string operateOn(struct SelectExpression * exp);
	std::string operateOn(struct SelectionStatement * stmnt);
	std::string operateOn(struct Statement * statement);
	std::string operateOn(struct Struct * strct);
	std::string operateOn(struct TypeSpecifier * spec);
	std::string operateOn(struct UnaryExpression * desc);
	std::string operateOn(struct UsingDeclaration * usingDecl);
	std::string operateOn(struct VariableAttribute * node);
	std::string operateOn(struct VariableDeclaration * node);
	std::string operateOn(struct VariableList * node);
	std::string operateOn(struct VariableNameDeclaration * node);

	std::string traverse(struct Node * node);

	std::string mapStructMember(const std::string & possibleStructMember) const;

private:
	unsigned int _outLocation, _inLocation, _uniformLocation;
	
	int _indent;
	FunctionDeclaration * _shader;
	VariableDeclaration * _inDecl; // what are the in variables?
	std::map<std::string,Struct*> _topLevelStructs; // for mapping in, out and uniform variables
	std::map<std::string, std::string> _structMemberMap;
	std::vector<VariableDeclaration* > _uniformVariables;
	TranspilerState _state;

	VariableDeclaration * getVariableFromName(const std::string & name);

	
	std::string indent();
	std::string mapIdentifier(const std::string & src) const;
	std::string mapToGLType(const struct VariableDeclaration * vDecl) const;
	std::string mapToGLType(const struct DeclarationSpecifierList * declSpecList, const BufferDescriptor * bufDesc) const;
	std::string mapToGLType(const struct DeclarationSpecifier * declSpec, const BufferDescriptor * bufDesc) const;
	std::string mapToGLType(const struct TypeSpecifier * typeSpec, const BufferDescriptor * bufDesc) const;
	std::string mapToGLType(const struct Qualifier * qualifier, const BufferDescriptor * bufDesc) const;
	std::string outputMain();
	std::string outputInOutUniforms();
	std::string outputIn();
	std::string outputOut();
	std::string outputUniforms();
	std::string outputToolbox();
	void categoriseVariableDeclaration(VariableDeclaration * vDecl);
	bool isSamplerGLType(const std::string & glType) const;
	bool isSimpleGLType(const std::string & glType) const;
	bool isSimpleGLType(VariableDeclaration * vDecl) const;
	std::string extractAttributeIndex(VariableDeclaration * vDecl);
	std::string createStructInitializer(struct Struct * strct, const std::string & initializations);	
	std::string baseOutVariableName() const;
	std::string toCommaSeparatedList(const std::vector<VariableNameDeclaration *> & input, bool mapInput);
	std::string addLocation(unsigned int & location);

};

#endif
