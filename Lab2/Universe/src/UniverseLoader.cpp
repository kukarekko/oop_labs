#include "UniverseLoader.h"
#include <iostream>
#include <limits>

Universe UniverseLoader::loadFromFile(const std::string& fileName) {
  FileReader reader(fileName);
  if (!reader.open())
    throw std::runtime_error("Cannot open file: " + fileName);

  std::string name = "Universe";
  std::string rule = "B3/S23";
  std::set<std::pair<int, int>> cells;

  bool hasName = false;
  bool hasRule = false;
  int duplicateCount = 0;
  int invalidLineCount = 0;

  if (!reader.readLine())
    throw std::runtime_error("Empty file: " + fileName);

  std::string firstLine = reader.getLine();
  if (firstLine != "#Life 1.06") {
    std::cout << "Warning: File does not start with '#Life 1.06', but: "
              << firstLine << std::endl;
  }

  while (reader.readLine()) {
    std::string line = reader.getLine();

    if (line.empty()) continue;

    processLine(line, name, rule, cells,
      hasName, hasRule, duplicateCount, invalidLineCount);
  }
  reader.close();

  if (!hasName)
    std::cout << "Warning: No universe name specified (#N line), "
              << "using default: '" << name << "'" << std::endl;

  if (!hasRule)
    std::cout << "Warning: No rule specified (#R line), "
              << "using default: '" << rule << "'" << std::endl;

  if (duplicateCount > 0)
    std::cout << "Warning: " << duplicateCount
              << " duplicate coordinate(s) ignored" << std::endl;

  if (invalidLineCount > 0)
    std::cout << "Warning: " << invalidLineCount
              << " invalid line(s) ignored" << std::endl;

  if (cells.empty())
    std::cout << "Warning: No live cells in file" << std::endl;

  auto [width, height] = calculateFieldSize(cells);

  Universe universe(width, height, name, rule);
  for (const auto& [x, y] : cells) {
    universe.setCell(x, y, true);
  }

  std::cout << "Loaded: " << name << " (" << cells.size() << " cells)" << std::endl;
  return universe;
}

void UniverseLoader::processLine(const std::string& line, std::string& name,
                                 std::string& rule, std::set<std::pair<int, int>>& cells,
                                 bool& hasName, bool& hasRule,
                                 int& duplicateCount, int& invalidLineCount) {

  if (Parser::isNameLine(line)) {
    std::string parsedName = Parser::parseName(line);
    if (!parsedName.empty()) {
      name = parsedName;
      hasName = true;
    } else {
      std::cout << "Warning: Empty universe name in line" << std::endl;
    }
  }
  else if (Parser::isRuleLine(line)) {
    std::string parsedRule = Parser::parseRule(line);
    if (!parsedRule.empty()) {
      rule = parsedRule;
      hasRule = true;
    } else {
      std::cout << "Warning: Empty rule in line" << std::endl;
    }
  }
  else if (Parser::isCoordinateLine(line)) {
    auto coords = Parser::parseCoordinates(line);

    if (cells.find(coords) != cells.end()) {
      duplicateCount++;
      std::cout << "Warning: Duplicate coordinates: "
                << coords.first << " " << coords.second << std::endl;
    } else {
      cells.insert(coords);
    }
  }
  else if (Parser::isCommentLine(line)) {
    return;
  }
  else if (!line.empty()) {
    invalidLineCount++;
    std::cout << "Warning: Invalid line ignored: '" << line << "'" << std::endl;
  }
}

std::pair<int, int> UniverseLoader::calculateFieldSize(
    const std::set<std::pair<int, int>>& cells) {
  if (cells.empty())
    return {20, 20};

  int maxX = std::numeric_limits<int>::lowest();
  int maxY = std::numeric_limits<int>::lowest();

  for (const auto& [x, y] : cells) {
    maxX = std::max(maxX, x);
    maxY = std::max(maxY, y);
  }

  int width = std::max(20, maxX + 10);
  int height = std::max(20, maxY + 10);

  return {width, height};
}