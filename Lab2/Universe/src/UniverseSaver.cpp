#include "UniverseSaver.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>

bool UniverseSaver::saveToFile(const Universe& universe,
                               const std::string& fileName) {
  FileWriter writer(fileName);
  if (!writer.open())
    return false;

  writeUniverse(universe, writer);
  writer.close();

  std::cout << "Saved: " << universe.getName()
              << " to " << fileName << "\n";
  return true;
}

void UniverseSaver::writeUniverse(const Universe& universe,
                                  FileWriter& writer) {
  writer.writeLine("#Life 1.06");

  std::string name = universe.getName();
  if (name.empty()) name = "Universe";

  writer.writeLine("#N " + name);
  writer.writeLine("#R " + universe.getRule());

  auto cells = universe.getLiveCells();
  if (cells.empty()) {
    writer.writeLine("0 0");
    return;
  }

  int minX = std::numeric_limits<int>::max();
  int minY = std::numeric_limits<int>::max();

  for (const auto& [x, y] : cells) {
    minX = std::min(minX, x);
    minY = std::min(minY, y);
  }

  for (const auto& [x, y] : cells) {
    std::ostringstream oss;
    oss << x << " " << y;
    writer.writeLine(oss.str());
  }
}

