#include "Reader.h"
#include <fstream>
#include <vector>

std::vector<std::string> readLines(const std::string& inputFile) {
    std::ifstream read_file(inputFile);
    if (!read_file.is_open()) {
        return std::vector<std::string>();
    }
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(read_file, line)) {
        lines.push_back(line);
    }
    read_file.close();
    return lines;
}

