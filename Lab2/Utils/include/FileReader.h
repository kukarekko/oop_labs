#ifndef LIFEFILEREADER_H
#define LIFEFILEREADER_H

#include <string>
#include <fstream>

class FileReader {
private:
    std::string fileName;
    std::ifstream fileStream;
    std::string currentLine;
    int lineNumber = 0;
public:
    explicit FileReader(const std::string& fileName);
    bool open();
    void close();
    bool readLine();
    bool isOpen() const;
    const std::string& getLine() const;
    int getLineNumber() const;
};
#endif
