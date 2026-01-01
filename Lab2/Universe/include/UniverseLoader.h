#ifndef UNIVERSELOADER_H
#define UNIVERSELOADER_H

#include "Universe.h"
#include "../Utils/FileReader.h"
#include "../Utils/Parser.h"

class UniverseLoader {
public:
    static Universe loadFromFile(const std::string& fileName);

private:
    static void processLine(const std::string& line, std::string& name,
        std::string& rule, std::set<std::pair<int, int>>& cells,
        bool& hasName, bool& hasRule, int& duplicateCount, int& invalidLineCount);

    static std::pair<int, int> calculateFieldSize(
        const std::set<std::pair<int, int>>& cells);
};



#endif //UNIVERSELOADER_H
