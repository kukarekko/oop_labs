#include "FileWriter.h"
#include <iostream>

FileWriter::FileWriter(const std::string& fileName)
    : fileName(fileName) {}

bool FileWriter::open() {
  fileStream.open(fileName);
  if (!fileStream.is_open()) {
     std::cerr << "Cannot create file: " << fileName << "\n";
     return false;
  }
  return true;
}

void FileWriter::close() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

bool FileWriter::isOpen() const {
    return fileStream.is_open();
}

bool FileWriter::writeLine(const std::string& line) {
    if (!fileStream.is_open()) {
        return false;
    }

    fileStream << line << "\n";
    return !fileStream.fail();
}
