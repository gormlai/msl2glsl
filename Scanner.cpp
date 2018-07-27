#include "Scanner.h"
#include <fstream>
#include <sstream>

Metal::Scanner * g_scanner = nullptr;

namespace
{
  const std::string extractPath(const std::string & filename,const std::string & separator) {
    std::string path;
    
    auto sepPos = filename.find_last_of(separator);
    if(sepPos != std::string::npos) {
      path = filename.substr(0, sepPos);
    }

    return path;
    
  }
}

Metal::Scanner::Scanner(const std::string & filename) : yyFlexLexer() {
  g_scanner = this;
  pushFile(filename);
  _path = extractPath(filename, getSeparator());
}

    
Metal::Scanner::~Scanner () {
  g_scanner = nullptr;
}

Metal::Scanner * Metal::Scanner::getInstance() {
  return g_scanner;
}

void Metal::Scanner::pushFile(const std::string & filename) {
  const std::string fullPath = !_path.empty() ? _path + getSeparator() + filename : filename; 

  std::ifstream * stream = new std::ifstream(fullPath); // TODO: leaks
  if(!stream->good()) {
    std::cerr << "Could not find file: " << fullPath << std::endl;
    delete stream;
    return;
  }
  
  yypush_buffer_state(yy_create_buffer(stream,2 << 16));
}

void Metal::Scanner::popFile() {
  yypop_buffer_state();
}

const std::string Metal::Scanner::getSeparator() const {
  return "/";
}

int Metal::Scanner::comment(int lines) {
  while(true) {
    const char c0 = yyinput();

	if (c0 == '\n')
		lines++;
    
    if(c0!='*' && c0!=0)
      continue;
    
    const char c1 = yyinput();
    if(c1!='/' && c1!=0)
      continue;
    
    break;
  }

  return lines;
  
}
