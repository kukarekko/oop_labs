#include "WriterFileCSV.h"
#include <fstream>
#include <vector>
#include <utility>

bool writeOutput(const std::string& outputFile,
    const std::vector<std::pair<std::string, int>>& sortedWords,
    int totalWords) {

    std::ofstream outFile(outputFile);
    if (!outFile.is_open())
        return false;

    for (const auto& pair : sortedWords) {
        double percentage = (static_cast<double>(pair.second) / totalWords) * 100.0;
        outFile << pair.first << ", " << pair.second << "   " << percentage << "%" << std::endl;
    }
    outFile.close();
    return true;
}

