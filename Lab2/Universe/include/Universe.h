#pragma once
#include <vector>
#include <set>
#include <string>
#include <stdexcept>
#include <utility>

class Universe {
public:
    Universe() : w(40), h(20), name("Universe"), rule("B3/S23"), generation(0) {
        parseRule(rule);
        initializeField();
    }

    Universe(int w, int h, const std::string& universeName = "Universe",
             const std::string& ruleString = "B3/S23");

    void tick(int n = 1);
    void clear();
    void randomize();
    void setCell(int x, int y, bool alive); //Установить состояние
    bool getCell(int x, int y) const; //Получить состояние

    std::set<std::pair<int, int>> getLiveCells() const;

    void setRule(const std::string& ruleString);
    void setName(const std::string& universeName) { name = universeName; }

    int width() const { return w; }
    int height() const { return h; }
    int getGeneration() const { return generation; }

    std::string getName() const { return name; }
    std::string getRule() const { return rule; }

    void print() const;

private:
    int w, h;
    std::string name;
    std::string rule;
    int generation;

    std::vector<int> birthRules;
    std::vector<int> survivalRules;

    void initializeField();
    void swapField();

    std::vector<std::vector<bool>> pastField;
    std::vector<std::vector<bool>> currentField;

    int countAlive(int x, int y) const;
    void parseRule(const std::string& ruleString);

    int normalizeX(int x) const;
    int normalizeY(int y) const;

    bool checkBirth(int neighbors) const;
    bool checkSurvival(int neighbors) const;
};
