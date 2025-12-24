#ifndef WRITEFILECSV_H
#define WRITEFILECSV_H

#include <string>
#include <vector>
#include <utility>

bool writeOutput(const std::string& outputFile,
                 const std::vector<std::pair<std::string, int>>& sortedWords,
                 int totalWords);

#endif
