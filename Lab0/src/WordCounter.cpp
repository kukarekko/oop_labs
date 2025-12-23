#include "WordCounter.h"
#include "CleanerWord.h"
#include "Reader.h"
#include "WriterFileCSV.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

WordCounter::WordCounter(const std::string& input, const std::string& output) {
    inputFile = input;
    outputFile = output;
    totalWords = 0;
}

bool compareWord(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
    return a.second > b.second;
}

void WordCounter::countWords(const std::vector<std::string>& lines) {
    totalWords = 0;
    wordCounter.clear();

    for (const std::string& currentLine : lines) {
        std::istringstream iss(currentLine);
        std::string word;
        while (iss >> word) {
            std::string clean = cleanWord(word);
            if (!clean.empty()) {
                wordCounter[clean]++;
                totalWords++;
            }
        }
    }
}

std::vector<std::pair<std::string, int>> WordCounter::sortWords() const {
    std::vector<std::pair<std::string, int>> sortedWords;
    for (const auto& element : wordCounter) {
        sortedWords.push_back(element);
    }
    std::sort(sortedWords.begin(), sortedWords.end(), compareWord);
    return sortedWords;
}

void WordCounter::process() {
    std::vector<std::string> lines = readLines(inputFile);
    if (lines.empty()) {
        std::cout << "Input file is empty" << std::endl;
        return;
    }

    countWords(lines);

    std::vector<std::pair<std::string, int>> sortedWords = sortWords();

    if (!writeOutput(outputFile, sortedWords, totalWords)) {
        std::cout << "Error writing output file" << std::endl;
        return;
    }

    std::cout << "Completed! " << totalWords << " words." << std::endl;
}
