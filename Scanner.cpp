#include "Scanner.h"

Metal::Scanner * g_scanner = nullptr;

Metal::Scanner::Scanner(std::istream *in) : yyFlexLexer(in) {
  g_scanner = this;
}

    
Metal::Scanner::~Scanner () {
  g_scanner = nullptr;
}

Metal::Scanner * Metal::Scanner::getInstance() {
  return g_scanner;
}


void Metal::Scanner::comment(int &lines) {
  while(true) {
    const char c0 = yyinput();

    if(c0=='\n')
      lines++;
    
    if(c0!='*' && c0!=0)
      continue;
    
    const char c1 = yyinput();
    if(c1!='/' && c1!=0)
      continue;
    
    break;
  }
  
}
