#ifndef _SCANNER_H_
#define _SCANNER_H_

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "metal.tab.h"
#include "location.hh"

namespace Metal
{
  class Scanner : public yyFlexLexer {
  public:
    Scanner(std::istream *in) : yyFlexLexer(in)
    {
    }

    virtual ~Scanner ();

    using FlexLexer::yylex;
    virtual int yylex(Metal::Parser::semantic_type * const lval, Metal::Parser::location_type * location);


  private:
    Metal::Parser::semantic_type * _yyval= nullptr;
  };
}

#endif
