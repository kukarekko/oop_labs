#include "ConfigParser.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Command> ConfigParser::parse(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw ConfigError("Cannot open config: " + path);
    }

    std::vector<Command> cmds;
    std::string line;

    while (std::getline(in, line)) {
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;

        std::istringstream ss(line);
        Command c;
        ss >> c.name;

        if (c.name.empty()) continue;

        std::string arg;
        while (ss >> arg) {
            c.args.push_back(arg);
        }
        cmds.push_back(c);
    }
    return cmds;
}
