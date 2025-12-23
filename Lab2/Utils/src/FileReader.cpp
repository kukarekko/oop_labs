#include "FileReader.h"
#include <iostream>

FileReader::FileReader(const std::string& fileName)
    : fileName(fileName) {}

bool FileReader::open() {
  if (fileStream.is_open())
    fileStream.close();
  fileStream.open(fileName, std::ios::in);

  if (!fileStream.is_open()) {
      std::cerr << "Cannot open file: " << fileName << "\n";
      return false;
  }

  lineNumber = 0;
  return true;
}

void FileReader::close() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
    currentLine.clear();
    lineNumber = 0;
}

bool FileReader::readLine() {
    if (!fileStream.is_open())
        return false;

    if (std::getline(fileStream, currentLine)) {
        ++lineNumber;
        return true;
    }
    return false;
}

bool FileReader::isOpen() const {
    return fileStream.is_open();
}

const std::string& FileReader::getLine() const {
    return currentLine;
}

int FileReader::getLineNumber() const {
    return lineNumber;
}
