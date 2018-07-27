WHITESPACE [ \t\v\f]
NEWLINE [\n]
LETTER [_a-zA-Z]
DIGIT [0-9]
HEX [0-8a-fA-F]
LETTER_OR_DIGIT ({LETTER}|{DIGIT})
FORWARDSLASH [/]
LETTER_OR_DIGIT_OR_FORWARDSLASH ({LETTER}|{DIGIT}|{FORWARDSLASH})

%{

#include <iostream>
#include "Types.h"
#include <string.h>
#include "Scanner.h"
#include <stdio.h>

#undef YY_DECL
#define YY_DECL int Metal::Scanner::yylex(Metal::Parser::semantic_type * const lval, Metal::Parser::location_type *loc)

using token = Metal::Parser::token;

struct ScanDescriptor
{
	int lines;
	std::string filename;
};

std::vector<ScanDescriptor> scanDescriptors = { {1, ""} };

void startNewFile(const std::string & file) 
{
	ScanDescriptor desc;
	desc.lines = 1;
	desc.filename = file;
	scanDescriptors.push_back(desc);
}

void fileEnded()
{
	scanDescriptors.pop_back();
}

void incrementLine()
{
	if(scanDescriptors.empty())
		return;

	const size_t lastIndex = scanDescriptors.size()-1;
	ScanDescriptor & desc = scanDescriptors[lastIndex];
	desc.lines++;
}

void setCurrentLine(int line) 
{
	if(scanDescriptors.empty())
		return;

	const size_t lastIndex = scanDescriptors.size()-1;
	ScanDescriptor & desc = scanDescriptors[lastIndex];
	desc.lines = line;
}

int currentLine() 
{
	if(scanDescriptors.empty())
		return 0;

	const size_t lastIndex = scanDescriptors.size()-1;
	ScanDescriptor & desc = scanDescriptors[lastIndex];
	return desc.lines;
}

std::string currentFile() 
{
	if(scanDescriptors.empty())
		return std::string("");

	const size_t lastIndex = scanDescriptors.size()-1;
	ScanDescriptor & desc = scanDescriptors[lastIndex];
	return desc.filename;
}


//#define yyterminate() printf("end!\n"); fflush(NULL); return(EOF )
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option debug
%option yyclass = "Metal::Parser"
%option c++
%option verbose
%option noyywrap
%x inclstate			
%%

%{
  _yyval = lval;
%}

"default"                                  { return token::DEFAULT; }
"switch"                                   { return token::SWITCH; }
"break"                                    { return token::BREAK; }
"continue"                                 { return token::CONTINUE; }
"case"                                     { return token::CASE; }
"fragment"                                 { return token::FRAGMENT; }
"kernel"                                   { return token::KERNEL; }
"vertex"                                   { return token::VERTEX; }
"static"                                   { return token::STATIC; }
"/*"                                       { setCurrentLine(Scanner::getInstance()->comment(currentLine())); }
"struct"                                   { return token::STRUCT;}
"constant"                                 { return token::CONSTANT;}
"const"                                    { return token::CONST;}
"constexpr"                                { return token::CONSTEXPR;}
"device"                                   { return token::DEVICE;}
"signed"                                   { return token::SIGNED;}
"unsigned"                                 { return token::UNSIGNED;}
"{"                                        { return token::BEGIN_CURLY_BRACKET; }
"}"                                        { return token::END_CURLY_BRACKET; }
"/"                                        { return token::FORWARD_SLASH; }
"<"                                        { return token::LESS_THAN; }
">"                                        { return token::GREATER_THAN; }
"<="                                       { return token::LESS_THAN_OR_EQUAL; }
">="                                       { return token::GREATER_THAN_OR_EQUAL; }
"=="                                       { return token::EQUAL_TO; }
"!="                                       { return token::NOT_EQUAL_TO; }
"="                                        { return token::ASSIGN_EQUAL; }
"+="                                       { return token::ASSIGN_PLUS; }
"-="                                       { return token::ASSIGN_MINUS; }
"*="                                       { return token::ASSIGN_MULTIPLY; }
"%="                                       { return token::ASSIGN_MODULO; }
"/="                                       { return token::ASSIGN_DIVIDE; }
"^="                                       { return token::ASSIGN_XOR; }
"&="                                       { return token::ASSIGN_AND; }
"|="                                       { return token::ASSIGN_OR; }
"using namespace"                          { return token::USING_NAMESPACE; }
";"                                        { return token::SEMICOLON; }
":"                                        { return token::COLON; }
"?"                                        { return token::QUESTION_MARK; }
","                                        { return token::COMMA; }
"+"                                        { return token::PLUS; }
"-"                                        { return token::MINUS; }
"*"                                        { return token::STAR; }
"%"                                        { return token::PERCENT; }
"|"                                        { return token::PIPE; }
"^"                                        { return token::HAT; }
"||"                                       { return token::DOUBLE_PIPE; }
"--"                                       { return token::MINUS_MINUS; }
"++"                                       { return token::PLUS_PLUS; }
"&"                                        { return token::AMPERSAND; }
"&&"                                       { return token::DOUBLE_AMPERSAND; }
"<<"                                       { return token::LEFT_SHIFT; }
">>"                                       { return token::RIGHT_SHIFT; }
"."                                        { return token::DOT; }
"->"                                       { return token::POINTER; }
"::"                                       { return token::DOUBLE_COLON; }
"access"                                   { return token::ACCESS; }
"("                                        { return token::BEGIN_BRACKET; }
")"                                        { return token::END_BRACKET; }
"["                                        { return token::BEGIN_SINGLE_SQUARE_BRACKET; }
"]"                                        { return token::END_SINGLE_SQUARE_BRACKET; }
"[["                                       { return token::BEGIN_DOUBLE_SQUARE_BRACKET; }
"]]"                                       { return token::END_DOUBLE_SQUARE_BRACKET; }
"return"                                   { return token::RETURN; }
"typedef"                                  { return token::TYPEDEF; }
"if"                                       { return token::IF; }
"else if"                                  { return token::ELSEIF; }
"else"                                     { return token::ELSE; }
"for"                                      { return token::FOR; }
{DIGIT}*"."{DIGIT}+"f"                     { std::string t(yytext,yyleng); _yyval->floatValue = atof(t.c_str()); return token::FLOAT_VALUE; }
{DIGIT}*"."{DIGIT}+"h"                     { std::string t(yytext,yyleng); _yyval->halfValue = atof(t.c_str()); return token::HALF_VALUE; }
{DIGIT}*"."{DIGIT}+                        { std::string t(yytext,yyleng); _yyval->doubleValue = (double)atof(t.c_str()); return token::DOUBLE_VALUE; }
{DIGIT}+"."{DIGIT}*                        { std::string t(yytext,yyleng); _yyval->doubleValue = (double)atof(t.c_str()); return token::DOUBLE_VALUE; }
{DIGIT}+                                   { std::string t(yytext,yyleng); _yyval->intValue = atoi(t.c_str()); return token::INT_VALUE; }
"0x"{HEX}+                                 { std::string t(yytext,yyleng); _yyval->string = new std::string(yytext,yyleng) ; return token::HEX_VALUE; }
"0x"{HEX}+"u"                              { std::string t(yytext,yyleng); _yyval->string = new std::string(yytext,yyleng) ; return token::HEX_VALUE; }
{DIGIT}+"u"                                { std::string t(yytext,yyleng); _yyval->intValue = atoi(t.c_str()); return token::INT_VALUE; /* TODO: does this need expansion with uints? */ }
{LETTER}{LETTER_OR_DIGIT}*                 { _yyval->string = new std::string(yytext,yyleng) ; return token::IDENTIFIER; }
{WHITESPACE}                               { /* skip */ }
{NEWLINE}                                  { incrementLine(); }
"#define"[^\n]*{NEWLINE}                   { incrementLine(); _yyval->string = new std::string(yytext,yyleng) ; return token::PREPROCESSOR; }
"#if"[^\n]*{NEWLINE}                       { incrementLine(); _yyval->string = new std::string(yytext,yyleng) ; return token::PREPROCESSOR; }
"#ifdef"[^\n]*{NEWLINE}                    { incrementLine(); _yyval->string = new std::string(yytext,yyleng) ; return token::PREPROCESSOR; }
"#else"[^\n]*{NEWLINE}                     { incrementLine(); _yyval->string = new std::string(yytext,yyleng) ; return token::PREPROCESSOR; }
"#elif"[^\n]*{NEWLINE}                     { incrementLine(); _yyval->string = new std::string(yytext,yyleng) ; return token::PREPROCESSOR; }
"#endif"[^\n]*{NEWLINE}                    { incrementLine(); _yyval->string = new std::string(yytext,yyleng) ; return token::PREPROCESSOR; }
"//"[^\n]*                                 {/* skip */ }
"#include"                                 BEGIN(inclstate);
<inclstate>{WHITESPACE}"<"[^\n]*">" { /* skip for now */;  BEGIN(0); }
<inclstate>{WHITESPACE}"\""[^\n]*"\"" { std::string t(yytext) ; auto startPos = t.find("\""); auto endPos = t.find_last_of("\""); t = t.substr(startPos+1, endPos - startPos-1);  Scanner::getInstance()->pushFile(t);  ; BEGIN(0); startNewFile(t); }


<<EOF>>                                    {  fileEnded(); Scanner::getInstance()->popFile(); if(!YY_CURRENT_BUFFER) yyterminate(); }

%%


void Metal::Parser::error(const location_type &unused, const std::string &err)
{
	int line = currentLine();
	std::string file = currentFile();
	if(file.empty())
	    std::cerr << "Error at line: " << line << ": " << err << std::endl; 
	else
	    std::cerr << "Error at line: " << line << " (" << file << "): " << err << std::endl; 
}
