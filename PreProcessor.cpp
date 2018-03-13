#include "PreProcessor.h"
#include <sstream>
#include <iostream>
#include <fstream>

Metal::PreProcessor::PreProcessor()
{
}

std::string Metal::PreProcessor::convertFile(const std::string & inFileName) {
  std::string result;
  std::ifstream file(inFileName.c_str(), std::ios::in | std::ios::binary);
  if(file) {
    file.seekg(0, std::ios::end);
    result.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&result[0], result.size());
    file.close();

  }
  
  return convertContents(result);
}

std::string Metal::PreProcessor::convertContents(const std::string & inData) {
  std::string outData;
  if(!inData.empty()) {
    std::string line;
    std::istringstream inStream(inData.c_str());
    int i=0;
    while(std::getline(inStream, line)) {
      i++;
      const std::string workCopy = line;
      //      std::cout << "Preprocessor examining line: " << line << std::endl;
      // TODO: remove all whitespaces first
      // TODO: possibly make lower case first
      const std::string searchFor("#include");
      std::size_t index = workCopy.find(searchFor);
      if(index != std::string::npos) {
	// possibly call recursively - for now just erase
	std::cout << "removing line: " << index << ", " << line  << std::endl;
      }
      else { // add line to outData
	outData += line + "\n";
      }
    }
  }

  return outData;
}
