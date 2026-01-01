#pragma once
#include <vector>
#include <string>

struct Command {
    std::string name;
    std::vector<std::string> args;
};

class ConfigParser {
public:
    static std::vector<Command> parse(const std::string& path);
};
