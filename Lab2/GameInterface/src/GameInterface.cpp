#include "GameInterface.h"
#include "Universe/UniverseLoader.h"
#include "Universe/UniverseSaver.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <thread>
#include <chrono>
#include <limits>
#include <windows.h>

void GameInterface::showTitle() {
    std::cout << "\n";
    showLine(50);
    std::cout << "      GAME OF LIFE\n";
    showLine(50);
    std::cout << "\n";
}

void GameInterface::showUniverse(const Universe& universe) {
    std::cout << "\n";
    universe.print();
    std::cout << "\n";
}

void GameInterface::showUniverseInfo(const Universe& universe) {
    std::cout << "\n";
    showLine(35);
    std::cout << " Info:\n";
    std::cout << " Name: " << universe.getName() << "\n";
    std::cout << " Rule: " << universe.getRule() << "\n";
    std::cout << " Size: " << universe.width() << " x " << universe.height() << "\n";
    std::cout << " Generation: " << universe.getGeneration() << "\n";
    std::cout << " Live cells: " << universe.getLiveCells().size() << "\n";
    showLine(35);
    std::cout << "\n";
}

void GameInterface::showHelp() {
    showTitle();
    std::cout << " COMMAND HELP:\n\n";

    std::cout << " Main commands:\n";
    std::cout << "   tick [n]  - make n steps\n";
    std::cout << "   dump file - save to file\n";
    std::cout << "   load file - load from file\n";
    std::cout << "   info      - show information\n";
    std::cout << "   exit      - quit\n\n";

    std::cout << " Editing:\n";
    std::cout << "   rule Bx/Sy    - change rule\n";
    std::cout << "   clear         - clear field\n";
    std::cout << "   random [%]    - random fill\n";
    std::cout << "   set x y       - make cell alive\n";
    std::cout << "   unset x y     - kill cell\n";
    std::cout << "   print         - print universe\n\n";

    std::cout << " Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void GameInterface::showDemoMenu() {
    std::cout << "\n";
    showLine(30);
    std::cout << " Select initial pattern:\n";
    std::cout << " 1. Glider\n";
    std::cout << " 2. Blinker\n";
    std::cout << " 3. Gosper Gun\n";
    std::cout << " 4. Random\n";
    showLine(30);
    std::cout << " Choice (1-4): ";
}

void GameInterface::showCommands() {
    std::cout << "\n";
    showLine(25);
    std::cout << " Commands: tick, dump, load,\n";
    std::cout << "          rule, clear, random,\n";
    std::cout << "          set, unset, info, exit\n";
    showLine(25);
    std::cout << " > ";
}

std::string GameInterface::getCommandPrompt() {
    std::string command;
    std::cout << "\n> ";
    std::getline(std::cin, command);
    return command;
}

void GameInterface::showMessage(const std::string& msg) {
    std::cout << " [i] " << msg << "\n";
}

void GameInterface::showError(const std::string& msg) {
    std::cout << " [X] Error: " << msg << "\n";
}

void GameInterface::showSuccess(const std::string& msg) {
    std::cout << " [✓] " << msg << "\n";
}

void GameInterface::showLoading(const std::string& msg) {
    std::cout << "\n " << msg;

    for (int i = 0; i < 3; i++) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void GameInterface::showLine(int length) {
    for (int i = 0; i < length; i++) {
        std::cout << "=";
    }
    std::cout << "\n";
}

void GameInterface::setupDemoPattern(Universe& universe) {
    showDemoMenu();

    std::string choice;
    std::getline(std::cin, choice);

    universe.clear();

    if (choice.empty() || choice == "1") {
        universe.setCell(1, 0, true);
        universe.setCell(2, 1, true);
        universe.setCell(0, 2, true);
        universe.setCell(1, 2, true);
        universe.setCell(2, 2, true);
        universe.setName("Glider");
        showMessage("Glider pattern loaded");
    }
    else if (choice == "2") {
        universe.setCell(1, 0, true);
        universe.setCell(1, 1, true);
        universe.setCell(1, 2, true);
        universe.setName("Blinker");
        showMessage("Blinker pattern loaded");
    }
    else if (choice == "3") {
        universe.setName("Gosper Gun Demo");
        int cx = universe.width() / 2 - 10;
        int cy = universe.height() / 2 - 5;

        universe.setCell(cx + 1, cy + 5, true);
        universe.setCell(cx + 2, cy + 5, true);
        universe.setCell(cx + 1, cy + 6, true);
        universe.setCell(cx + 2, cy + 6, true);

        universe.setCell(cx + 35, cy + 3, true);
        universe.setCell(cx + 36, cy + 3, true);
        universe.setCell(cx + 35, cy + 4, true);
        universe.setCell(cx + 36, cy + 4, true);
        showMessage("Gosper Glider Gun loaded");
    }
    else if (choice == "4") {
        universe.randomize();
        universe.setName("Random Universe");
        showMessage("Random pattern loaded");
    }
    else {
        universe.setCell(1, 0, true);
        universe.setCell(2, 1, true);
        universe.setCell(0, 2, true);
        universe.setCell(1, 2, true);
        universe.setCell(2, 2, true);
        universe.setName("Glider");
        showMessage("Default glider pattern loaded");
    }
}

bool GameInterface::processCommand(Universe& universe, const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "dump" || cmd == "save") {
        std::string filename;
        if (iss >> filename) {
            if (UniverseSaver::saveToFile(universe, filename)) {
                showSuccess("Universe saved to: " + filename);
            } else {
                showError("Failed to save universe");
            }
        } else {
            showError("Usage: dump <filename>");
            showMessage("Example: dump my_universe.lif");
        }
    }
    else if (cmd == "tick") {
        int n = 1;
        if (iss >> n) {
            if (n > 0) {
                showMessage("Before " + std::to_string(n) + " iteration(s):");
                std::cout << " Name: " << universe.getName() << "\n";
                std::cout << " Rule: " << universe.getRule() << "\n";
                std::cout << " Generation: " << universe.getGeneration() << "\n";

                showLoading("Executing " + std::to_string(n) + " iterations");

                universe.tick(n);

                showMessage("After " + std::to_string(n) + " iteration(s):");
                std::cout << " Name: " << universe.getName() << "\n";
                std::cout << " Rule: " << universe.getRule() << "\n";
                std::cout << " Generation: " << universe.getGeneration() << "\n";

                showUniverse(universe);
            } else {
                showError("Number of iterations must be positive");
            }
        } else {
            showMessage("Before 1 iteration:");
            std::cout << " Name: " << universe.getName() << "\n";
            std::cout << " Rule: " << universe.getRule() << "\n";
            std::cout << " Generation: " << universe.getGeneration() << "\n";

            universe.tick();

            showMessage("After 1 iteration:");
            std::cout << " Name: " << universe.getName() << "\n";
            std::cout << " Rule: " << universe.getRule() << "\n";
            std::cout << " Generation: " << universe.getGeneration() << "\n";
            showUniverse(universe);
        }
    }
    else if (cmd == "exit" || cmd == "quit") {
        showMessage("Exiting...");
        return false;
    }
    else if (cmd == "help" || cmd == "?") {
        showHelp();
    }
    else if (cmd == "info") {
        showUniverseInfo(universe);
    }
    else if (cmd == "rule") {
        std::string rule;
        if (iss >> rule) {
            try {
                universe.setRule(rule);
                showSuccess("Rule changed to: " + rule);
            } catch (const std::exception& e) {
                showError(e.what());
            }
        } else {
            showError("Usage: rule <Bx/Sy>");
            showMessage("Example: rule B3/S23 (default)");
            showMessage("Example: rule B36/S23 (HighLife)");
        }
    }
    else if (cmd == "clear") {
        universe.clear();
        showSuccess("Universe cleared");
        showUniverse(universe);
    }
    else if (cmd == "random") {
        int percent = 30;
        if (iss >> percent) {
            if (percent >= 0 && percent <= 100) {
                std::srand(std::time(nullptr));
                universe.clear();
                for (int y = 0; y < universe.height(); ++y) {
                    for (int x = 0; x < universe.width(); ++x) {
                        if ((std::rand() % 100) < percent) {
                            universe.setCell(x, y, true);
                        }
                    }
                }
                showSuccess("Random fill (" + std::to_string(percent) + "% live cells)");
                showUniverse(universe);
            } else {
                showError("Percentage must be between 0 and 100");
            }
        } else {
            universe.randomize();
            showSuccess("Random fill (30% live cells)");
            showUniverse(universe);
        }
    }
    else if (cmd == "set") {
        int x, y;
        if (iss >> x >> y) {
            if (x >= 0 && x < universe.width() && y >= 0 && y < universe.height()) {
                universe.setCell(x, y, true);
                showSuccess("Cell (" + std::to_string(x) + ", " + std::to_string(y) + ") set to alive");
            } else {
                showError("Coordinates out of bounds");
                showMessage("Valid range: x: 0-" + std::to_string(universe.width()-1) +
                           ", y: 0-" + std::to_string(universe.height()-1));
            }
        } else {
            showError("Usage: set <x> <y>");
        }
    }
    else if (cmd == "unset" || cmd == "remove") {
        int x, y;
        if (iss >> x >> y) {
            if (x >= 0 && x < universe.width() && y >= 0 && y < universe.height()) {
                universe.setCell(x, y, false);
                showSuccess("Cell (" + std::to_string(x) + ", " + std::to_string(y) + ") set to dead");
            } else {
                showError("Coordinates out of bounds");
                showMessage("Valid range: x: 0-" + std::to_string(universe.width()-1) +
                           ", y: 0-" + std::to_string(universe.height()-1));
            }
        } else {
            showError("Usage: unset <x> <y>");
        }
    }
    else if (cmd == "print" || cmd == "show") {
        showUniverse(universe);
    }
    else if (cmd == "load" || cmd == "open") {
        std::string filename;
        if (iss >> filename) {
            try {
                universe = UniverseLoader::loadFromFile(filename);
                showSuccess("Universe loaded from: " + filename);
                showUniverseInfo(universe);
                showUniverse(universe);
            } catch (const std::exception& e) {
                showError("Failed to load file: " + std::string(e.what()));
                showMessage("Make sure the file exists in the current directory");
                showMessage("Current directory: " + std::string(getenv("CD") ? getenv("CD") : "."));
            }
        } else {
            showError("Usage: load <filename>");
            showMessage("Example: load pattern.lif");
        }
    }
    else if (cmd == "reset" || cmd == "demo") {
        setupDemoPattern(universe);
        showSuccess("Demo pattern loaded");
        showUniverse(universe);
    }
    else if (!cmd.empty()) {
        showError("Unknown command: '" + cmd + "'");
        showMessage("Type 'help' for available commands");
    }

    return true;
}

void GameInterface::offlineMode(const std::string& inputFile,
                               const std::string& outputFile,
                               int iterations) {
    try {
        showTitle();
        showMessage("Offline mode:");
        showMessage("  Input: " + inputFile);
        showMessage("  Output: " + outputFile);
        showMessage("  Iterations: " + std::to_string(iterations));

        Universe universe = UniverseLoader::loadFromFile(inputFile);
        showMessage("Universe loaded:");
        showUniverse(universe);

        if (iterations > 0) {
            showLoading("Executing " + std::to_string(iterations) + " iterations");
            universe.tick(iterations);
        }

        if (UniverseSaver::saveToFile(universe, outputFile)) {
            showSuccess("Saved to " + outputFile);
        } else {
            showError("Save failed");
        }

    } catch (const std::exception& e) {
        showError("Offline mode error: " + std::string(e.what()));
        exit(1);
    }
}

void GameInterface::interactiveMode(Universe& universe) {
    showTitle();
    showUniverse(universe);
    showMessage("Type 'help' for commands");

    bool running = true;
    while (running) {
        std::string command = getCommandPrompt();

        if (command.empty()) {
            continue;
        }

        running = processCommand(universe, command);
    }
}

void GameInterface::promptSaveBeforeExit(Universe& universe) {
    showMessage("Save universe before exit? (y/n): ");
    std::string command;
    std::getline(std::cin, command);
    
    if (command == "y" || command == "Y") {
        showMessage("Enter filename (default 'autosave.lif'): ");
        std::getline(std::cin, command);
        if (command.empty()) command = "autosave.lif";
        
        if (UniverseSaver::saveToFile(universe, command)) {
            showSuccess("Saved to " + command);
        } else {
            showError("Save failed");
        }
    }
}

CommandLineArgs GameInterface::parseArguments(int argc, char* argv[]) {
    CommandLineArgs args;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            args.help = true;
            return args;
        }
        else if (arg == "-i" || arg == "--iterations") {
            if (i + 1 < argc) {
                try {
                    args.iterations = std::stoi(argv[++i]);
                    args.offline = true;
                } catch (...) {
                    showError("Invalid number of iterations");
                    args.help = true;
                    return args;
                }
            }
        }
        else if (arg.find("--iterations=") == 0) {
            try {
                args.iterations = std::stoi(arg.substr(13));
                args.offline = true;
            } catch (...) {
                showError("Invalid number of iterations");
                args.help = true;
                return args;
            }
        }
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                args.outputFile = argv[++i];
                args.offline = true;
            }
        }
        else if (arg.find("--output=") == 0) {
            args.outputFile = arg.substr(9);
            args.offline = true;
        }
        else if (arg[0] != '-') {
            args.inputFile = arg;
        }
        else {
            showError("Unknown option: " + arg);
            args.help = true;
            return args;
        }
    }

    return args;
}

void GameInterface::printUsage(const char* programName) {
    showTitle();
    std::cout << "\n";
    showLine(60);
    std::cout << " USAGE:\n\n";
    std::cout << "   " << programName << " [options] [input_file]\n\n";

    std::cout << " OPTIONS:\n";
    std::cout << "   -h, --help           Show this help message\n";
    std::cout << "   -i N, --iterations=N Number of iterations (offline mode)\n";
    std::cout << "   -o FILE, --output=FILE Output file (offline mode)\n\n";

    std::cout << " MODES:\n";
    std::cout << "   1. No arguments      Interactive mode with demo pattern\n";
    std::cout << "   2. Input file only   Load universe and interactive mode\n";
    std::cout << "   3. With -i/-o        Offline batch processing\n\n";

    std::cout << " EXAMPLES:\n";
    std::cout << "   " << programName << "                    Interactive demo\n";
    std::cout << "   " << programName << " glider.lif         Load and interactive\n";
    std::cout << "   " << programName << " -i 100 gun.lif     100 iterations\n";
    std::cout << "   " << programName << " -i 50 -o out.lif pattern.lif  Full offline\n";
    showLine(60);
}
