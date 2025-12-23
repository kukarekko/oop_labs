#include "Universe/Universe.h"
#include "GameInterface/GameInterface.h"
#include "Universe/UniverseLoader.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <windows.h>


int main(int argc, char* argv[]) {

    std::srand(std::time(nullptr));

    CommandLineArgs args = GameInterface::parseArguments(argc, argv);

    if (args.help) {
        GameInterface::printUsage(argv[0]);
        return 0;
    }

    if (args.offline) {
        if (args.inputFile.empty()) {
            GameInterface::showError("Offline mode requires input file");
            GameInterface::printUsage(argv[0]);
            return 1;
        }
        if (args.outputFile.empty()) {
            args.outputFile = "output.lif";
        }
        GameInterface::offlineMode(args.inputFile, args.outputFile, args.iterations);
        return 0;
    }

    Universe universe(40, 20, "Game of Life", "B3/S23");

    GameInterface::showTitle();

    if (!args.inputFile.empty()) {
        std::ifstream testFile(args.inputFile);
        if (!testFile.good()) {
            GameInterface::showError("File '" + args.inputFile + "' not found");
            GameInterface::setupDemoPattern(universe);
        } else {
            testFile.close();
            try {
                universe = UniverseLoader::loadFromFile(args.inputFile);
                GameInterface::showSuccess("Loaded from: " + args.inputFile);
            } catch (const std::exception& e) {
                GameInterface::showError("Load failed: " + std::string(e.what()));
                GameInterface::showMessage("Starting demo mode...");
                GameInterface::setupDemoPattern(universe);
            }
        }
    } else {
        GameInterface::setupDemoPattern(universe);
    }
    GameInterface::interactiveMode(universe);
    GameInterface::promptSaveBeforeExit(universe);
    GameInterface::showMessage("Goodbye!");
    return 0;
}