#include "CSVParser.h"
#include "Tuple.h"
#include <iostream>
#include <fstream>

int main(const int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Input file was expected";
        return 1;
    }
    std::ifstream file;
    file.open(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << argv[1] << std::endl;
        return 1;
    }
    try {
        CSVParser<int, std::string, int> parser(file, 1);
        for (const auto& row : parser) {
            std::cout << row << std::endl; //<<: print tuple
        }
    } catch (const CSVError& e) {
        std::cerr << "[CSV Error]: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}