#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include "../Universe/Universe.h"
#include <string>

struct CommandLineArgs {
    std::string inputFile;
    std::string outputFile;
    int iterations = 0;
    bool offline = false;
    bool help = false;
};

class GameInterface {
public:
    static CommandLineArgs parseArguments(int argc, char* argv[]);
    static void printUsage(const char* programName);

    static void showTitle();
    static void showUniverse(const Universe& universe);
    static void showUniverseInfo(const Universe& universe);

    static void showHelp();
    static void showDemoMenu();
    static void showCommands();

    static void showMessage(const std::string& msg);
    static void showError(const std::string& msg);
    static void showSuccess(const std::string& msg);

    static void showLoading(const std::string& msg);

    static void clearScreen();
    static void showLine(int length = 40);

    static std::string getCommandPrompt();
    static bool processCommand(Universe& universe, const std::string& command);
    static void setupDemoPattern(Universe& universe);
    static void offlineMode(const std::string& inputFile,
                           const std::string& outputFile,
                           int iterations);
    static void interactiveMode(Universe& universe);
    static void promptSaveBeforeExit(Universe& universe);
};

#endif