#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

class WordCounter {
private:
    std::string inputFile;
    std::string outputFile;
    std::map<std::string, int> wordCounter;
    int totalWords;

    void countWords(const std::vector<std::string>& lines);
    std::vector<std::pair<std::string, int>> sortWords() const;

public:
    WordCounter(const std::string& input, const std::string& output);
    void process();

    void testCountWords(const std::vector<std::string>& lines) { countWords(lines); }
    std::map<std::string, int> getWordCounter() const { return wordCounter; }
    int getTotalWords() const { return totalWords; }
    void resetCounter() {
        wordCounter.clear();
        totalWords = 0;
    }
};

#endif

