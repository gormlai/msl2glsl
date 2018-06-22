#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "Types.h"

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif


#include "metal.tab.h"
#include "location.hh"


namespace Metal
{
  
  class Scanner : public yyFlexLexer {
  public:
    Scanner(const std::string & filename);    
    virtual ~Scanner();

    using FlexLexer::yylex;
    virtual int yylex(Metal::Parser::semantic_type * const lval, Metal::Parser::location_type * location);
    void comment(int &lines);
    static Scanner * getInstance();
    void pushFile(const std::string & filename);
    void popFile();
    const std::string getSeparator() const;

  private:
    Metal::Parser::semantic_type * _yyval= nullptr;
    std::string _path;
  };
}

#endif
