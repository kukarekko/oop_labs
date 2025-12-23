#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <utility>


class Parser {
public:
    static bool isNameLine(const std::string& line);
    static bool isRuleLine(const std::string& line);
    static bool isCoordinateLine(const std::string& line);
    static bool isCommentLine(const std::string& line);

    static std::string parseName(const std::string& line);
    static std::string parseRule(const std::string& line);
    static std::pair<int, int> parseCoordinates(const std::string& line);
};
#endif //PARSER_H
