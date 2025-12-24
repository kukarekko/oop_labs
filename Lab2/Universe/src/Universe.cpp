#include "Universe.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

Universe::Universe(int w, int h, const std::string& universeName,
                   const std::string& ruleString)
    : w(w), h(h), name(universeName), generation(0) {
    parseRule(ruleString);
    initializeField();
}

void Universe::initializeField() {
    pastField.resize(h, std::vector<bool>(w, false));
    currentField.resize(h, std::vector<bool>(w, false));
}

void Universe::tick(int n) {
    for (int i = 0; i < n; ++i) {
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                int neighbors = countAlive(x, y);
                bool alive = currentField[y][x];

                if (alive) {
                    pastField[y][x] = checkSurvival(neighbors);
                } else {
                    pastField[y][x] = checkBirth(neighbors);
                }
            }
        }
        swapField();
        ++generation;
    }
}

void Universe::clear() {
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            currentField[y][x] = false;
        }
    }
    generation = 0;
}

void Universe::randomize() {
    std::srand(std::time(nullptr));
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            currentField[y][x] = (std::rand() % 100) < 30;
        }
    }
    generation = 0;
}

void Universe::setCell(int x, int y, bool alive) {
    int nx = normalizeX(x);
    int ny = normalizeY(y);
    currentField[ny][nx] = alive;
}

bool Universe::getCell(int x, int y) const {
    int nx = normalizeX(x);
    int ny = normalizeY(y);
    return currentField[ny][nx];
}

std::set<std::pair<int, int>> Universe::getLiveCells() const {
    std::set<std::pair<int, int>> cells;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (currentField[y][x]) {
                cells.insert({x, y});
            }
        }
    }
    return cells;
}

void Universe::setRule(const std::string& ruleString) {
    parseRule(ruleString);
}

void Universe::print() const {
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            std::cout << (currentField[y][x] ? 'O' : '.');
        }
        std::cout << "\n";
    }
}

void Universe::swapField() {
    std::swap(pastField, currentField);
}

int Universe::countAlive(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;

            int nx = normalizeX(x + dx);
            int ny = normalizeY(y + dy);

            if (currentField[ny][nx]) {
                ++count;
            }
        }
    }
    return count;
}

void Universe::parseRule(const std::string& ruleString) {
    rule = ruleString;
    birthRules.clear();
    survivalRules.clear();

    size_t slashPos = ruleString.find('/');
    if (slashPos == std::string::npos) {
        throw std::invalid_argument("Invalid rule format: " + ruleString);
    }

    std::string birthPart = ruleString.substr(0, slashPos);
    std::string survivalPart = ruleString.substr(slashPos + 1);

    if (birthPart.empty() || birthPart[0] != 'B') {
        throw std::invalid_argument("Invalid birth rule: " + birthPart);
    }
    if (survivalPart.empty() || survivalPart[0] != 'S') {
        throw std::invalid_argument("Invalid survival rule: " + survivalPart);
    }

    for (size_t i = 1; i < birthPart.size(); ++i) {
        if (isdigit(birthPart[i])) {
            birthRules.push_back(birthPart[i] - '0');
        }
    }

    for (size_t i = 1; i < survivalPart.size(); ++i) {
        if (isdigit(survivalPart[i])) {
            survivalRules.push_back(survivalPart[i] - '0');
        }
    }
}

int Universe::normalizeX(int x) const {
    if (x < 0) return (x % w + w) % w;
    if (x >= w) return x % w;
    return x;
}

int Universe::normalizeY(int y) const {
    if (y < 0) return (y % h + h) % h;
    if (y >= h) return y % h;
    return y;
}

bool Universe::checkBirth(int neighbors) const {
    for (int rule : birthRules) {
        if (neighbors == rule) {
            return true;
        }
    }
    return false;
}

bool Universe::checkSurvival(int neighbors) const {
    for (int rule : survivalRules) {
        if (neighbors == rule) {
            return true;
        }
    }
    return false;
}
