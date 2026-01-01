#include "Parser.h"
#include <iostream>
#include <sstream>

bool Parser::isNameLine(const std::string& line) {
    return line.find("#N") == 0;
}

bool Parser::isRuleLine(const std::string& line) {
    return line.find("#R") == 0;
}

bool Parser::isCoordinateLine(const std::string& line) {
  if (line.empty() || line[0] == '#')
    return false;

  std::istringstream iss(line);
  int x, y;
  return (iss >> x >> y) ? true : false;
}

bool Parser::isCommentLine(const std::string& line) {
    return !line.empty() && line[0] == '#';
}

std::string Parser::parseName(const std::string& line) {
    if (!isNameLine(line)) return "";
    size_t pos = line.find(' ');
    if (pos == std::string::npos) {
        if (line.length() > 2) {
            return line.substr(2);
        }
        return "";
    }

    return line.substr(pos + 1);
}

std::string Parser::parseRule(const std::string& line) {
    if (!isRuleLine(line)) return "";
    size_t rPos = line.find("#R");

    if (rPos == std::string::npos) return "";
    size_t start = rPos + 2;

    while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
        start++;
    if (start >= line.size()) return "";
    size_t end = line.size();

    while (end > start && std::isspace(static_cast<unsigned char>(line[end - 1])))
        end--;
    return line.substr(start, end - start);
}

std::pair<int, int> Parser::parseCoordinates(const std::string& line) {
    std::pair<int, int> coords = {0, 0};

    if (!isCoordinateLine(line)) {
        return coords;
    }

    std::istringstream iss(line);
    iss >> coords.first >> coords.second;

    return coords;
}

