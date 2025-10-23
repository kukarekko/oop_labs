#include <iostream>
#include <string>
#include "WordCounter.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        std::cerr << "Example: " << argv[0] << " input.txt output.csv" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    WordCounter counter(inputFile, outputFile);
    counter.process();
    return 0;
}

