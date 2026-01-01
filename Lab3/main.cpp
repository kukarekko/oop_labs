#include <iostream>
#include <string>
#include <vector>
#include "process/ProcessManager.h"
#include "Exceptions.h"

int main(int argc, char* argv[]) {
    try {
        if (argc == 2 && std::string(argv[1]) == "-h") {
            ProcessManager::printHelp();
            return 0;
        }

        if (argc < 4 || std::string(argv[1]) != "-c") {
            throw CmdLineError("Invalid arguments. Use -h for help.");
        }

        std::string configPath = argv[2];
        std::string outputPath = argv[3];
        std::vector<std::string> inputPaths;
        for (int i = 4; i < argc; ++i) {
            inputPaths.push_back(argv[i]);
        }

        ProcessManager processor;
        processor.process(configPath, outputPath, inputPaths);

    } catch (const SoundException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}
