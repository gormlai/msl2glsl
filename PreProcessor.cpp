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
  }

  return outData;
}
