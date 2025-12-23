#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>
#include <fstream>

class FileWriter {
private:
    std::string fileName;
    std::ofstream fileStream;
public:
    explicit FileWriter(const std::string& fileName);

    bool open();
    void close();
    bool isOpen() const;

    bool writeLine(const std::string& line);
};



#endif //FILEWRITER_H
