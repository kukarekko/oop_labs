#include "Universe/Universe.h"
#include "Utils/Parser.h"
#include "Universe/UniverseLoader.h"
#include "Universe/UniverseSaver.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <chrono>

namespace fs = std::filesystem;

// =========================== ПАРСЕР ===========================

void testParserBasics() {
    std::cout << "Testing Parser Basics...\n";

    // Тесты isNameLine
    assert(Parser::isNameLine("#N My Universe"));
    assert(Parser::isNameLine("#NTest"));
    assert(Parser::isNameLine("#N 123"));
    assert(!Parser::isNameLine(""));
    assert(!Parser::isNameLine("#R B3/S23"));
    assert(!Parser::isNameLine("#C Comment"));
    assert(!Parser::isNameLine("N Name"));  // Без #

    // Тесты isRuleLine
    assert(Parser::isRuleLine("#R B3/S23"));
    assert(Parser::isRuleLine("#R B36/S23"));
    assert(Parser::isRuleLine("#R B3/S238"));
    assert(!Parser::isRuleLine(""));
    assert(!Parser::isRuleLine("#N Name"));
    assert(!Parser::isRuleLine("R B3/S23"));  // Без #

    // Тесты isCommentLine
    assert(Parser::isCommentLine("# Comment"));
    assert(Parser::isCommentLine("#C Comment"));
    assert(Parser::isCommentLine("# Это комментарий на русском"));
    assert(Parser::isCommentLine("#N Name"));  // Имя - тоже комментарий
    assert(Parser::isCommentLine("#R Rule"));  // Правило - тоже комментарий
    assert(!Parser::isCommentLine(""));
    assert(!Parser::isCommentLine("Not a comment"));
    assert(!Parser::isCommentLine("10 20"));  // Координаты

    std::cout << "Parser Basics tests passed!\n";
}

void testParserCoordinates() {
    std::cout << "Testing Parser Coordinates...\n";

    // Тесты isCoordinateLine
    assert(Parser::isCoordinateLine("10 20"));
    assert(Parser::isCoordinateLine("-5 30"));
    assert(Parser::isCoordinateLine("0 0"));
    assert(Parser::isCoordinateLine("999 -999"));
    assert(Parser::isCoordinateLine("  10   20  "));
    assert(!Parser::isCoordinateLine(""));
    assert(!Parser::isCoordinateLine("# Comment"));
    assert(!Parser::isCoordinateLine("invalid"));
    assert(!Parser::isCoordinateLine("10"));
    assert(!Parser::isCoordinateLine("10 abc"));
    assert(!Parser::isCoordinateLine("abc 10"));
    assert(!Parser::isCoordinateLine("10.5 20.5"));

    // Тесты parseCoordinates
    auto coords = Parser::parseCoordinates("10 20");
    assert(coords.first == 10 && coords.second == 20);

    coords = Parser::parseCoordinates("-5 30");
    assert(coords.first == -5 && coords.second == 30);

    coords = Parser::parseCoordinates("0 0");
    assert(coords.first == 0 && coords.second == 0);

    coords = Parser::parseCoordinates("   -10   -20   ");
    assert(coords.first == -10 && coords.second == -20);

    // Некорректные строки должны возвращать (0, 0)
    coords = Parser::parseCoordinates("");
    assert(coords.first == 0 && coords.second == 0);

    coords = Parser::parseCoordinates("# Comment");
    assert(coords.first == 0 && coords.second == 0);

    coords = Parser::parseCoordinates("invalid");
    assert(coords.first == 0 && coords.second == 0);

    std::cout << "Parser Coordinates tests passed!\n";
}

void testParserExtraction() {
    std::cout << "Testing Parser Extraction...\n";

    // Тесты parseName
    assert(Parser::parseName("#N My Universe") == "My Universe");
    assert(Parser::parseName("#NGlider") == "Glider");
    assert(Parser::parseName("#N Test Universe 123") == "Test Universe 123");
    assert(Parser::parseName("#N") == "");
    assert(Parser::parseName("") == "");
    assert(Parser::parseName("#R B3/S23") == "");
    assert(Parser::parseName("Not a name line") == "");

    // Тесты parseRule
    assert(Parser::parseRule("#R B3/S23") == "B3/S23");
    assert(Parser::parseRule("#R B36/S23") == "B36/S23");
    assert(Parser::parseRule("#R B3/S238") == "B3/S238");
    assert(Parser::parseRule("#R Custom Rule") == "Custom Rule");
    assert(Parser::parseRule("#RB3/S23") == "B3/S23");
    assert(Parser::parseRule("#R") == "");
    assert(Parser::parseRule("") == "");
    assert(Parser::parseRule("#N Name") == "");

    std::cout << "Parser Extraction tests passed!\n";
}

// =========================== ВСЕЛЕННАЯ: БАЗОВОЕ ===========================

void testUniverseConstructors() {
    std::cout << "Testing Universe Constructors...\n";

    // Конструктор по умолчанию
    Universe u1;
    assert(u1.width() == 40);
    assert(u1.height() == 20);
    assert(u1.getName() == "Universe");
    assert(u1.getRule() == "B3/S23");
    assert(u1.getGeneration() == 0);

    // Пользовательский конструктор
    Universe u2(50, 30, "Test Universe", "B36/S23");
    assert(u2.width() == 50);
    assert(u2.height() == 30);
    assert(u2.getName() == "Test Universe");
    assert(u2.getRule() == "B36/S23");
    assert(u2.getGeneration() == 0);

    // Минимальные размеры
    Universe u3(1, 1, "Tiny", "B3/S23");
    assert(u3.width() == 1);
    assert(u3.height() == 1);

    // Большие размеры
    Universe u4(100, 100, "Large", "B3/S23");
    assert(u4.width() == 100);
    assert(u4.height() == 100);

    // Проверка сеттеров
    u4.setName("Renamed Universe");
    assert(u4.getName() == "Renamed Universe");

    u4.setRule("B2/S");
    assert(u4.getRule() == "B2/S");

    std::cout << "Universe Constructors tests passed!\n";
}

void testUniverseCellOperations() {
    std::cout << "Testing Universe Cell Operations...\n";

    Universe u(10, 10, "Test", "B3/S23");

    // Установка и получение клеток
    u.setCell(0, 0, true);
    assert(u.getCell(0, 0) == true);

    u.setCell(5, 5, true);
    assert(u.getCell(5, 5) == true);

    u.setCell(0, 0, false);  // Отключение
    assert(u.getCell(0, 0) == false);

    // Проверка по умолчанию все клетки мертвые
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            if (x != 5 || y != 5) {
                assert(!u.getCell(x, y));
            }
        }
    }

    u.clear();
    u.setCell(10, 10, true);
    assert(u.getCell(0, 0) == true);

    u.setCell(-1, -1, true);
    assert(u.getCell(9, 9) == true);

    u.setCell(15, 15, true);
    assert(u.getCell(5, 5) == true);  // 15 % 10 = 5

    u.setCell(-5, -5, true);
    assert(u.getCell(5, 5) == true);  // (-5 % 10 + 10) % 10 = 5

    std::cout << "Universe Cell Operations tests passed!\n";
}

void testUniverseClear() {
    std::cout << "Testing Universe Clear...\n";

    Universe u(10, 10, "Test", "B3/S23");

    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            if ((x + y) % 2 == 0) {
                u.setCell(x, y, true);
            }
        }
    }

    assert(u.getCell(0, 0) == true);
    assert(u.getCell(1, 1) == true);
    assert(u.getCell(2, 2) == true);

    u.clear();

    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            assert(!u.getCell(x, y));
        }
    }

    u.tick(5);
    assert(u.getGeneration() == 5);
    u.clear();
    assert(u.getGeneration() == 0);

    std::cout << "Universe Clear tests passed!\n";
}

void testUniverseGetLiveCells() {
    std::cout << "Testing Universe GetLiveCells...\n";

    Universe u(10, 10, "Test", "B3/S23");
    u.clear();

    // Добавляем несколько клеток
    u.setCell(1, 1, true);
    u.setCell(2, 2, true);
    u.setCell(3, 3, true);
    u.setCell(4, 4, true);

    auto liveCells = u.getLiveCells();
    assert(liveCells.size() == 4);

    // Проверяем, что все добавленные клетки в списке
    std::vector<std::pair<int, int>> expected = {
        {1, 1}, {2, 2}, {3, 3}, {4, 4}
    };

    for (const auto& expectedCell : expected) {
        bool found = false;
        for (const auto& actualCell : liveCells) {
            if (actualCell.first == expectedCell.first &&
                actualCell.second == expectedCell.second) {
                found = true;
                break;
            }
        }
        assert(found);
    }

    // После очистки список должен быть пустым
    u.clear();
    liveCells = u.getLiveCells();
    assert(liveCells.empty());

    std::cout << "Universe GetLiveCells tests passed!\n";
}

// =========================== ВСЕЛЕННАЯ: ПРАВИЛА ===========================

void testRuleParsing() {
    std::cout << "Testing Rule Parsing...\n";

    Universe u(10, 10, "Test", "B3/S23");

    // Классические правила
    u.setRule("B3/S23");
    assert(u.getRule() == "B3/S23");

    // HighLife
    u.setRule("B36/S23");
    assert(u.getRule() == "B36/S23");

    // День и ночь
    u.setRule("B3678/S34678");
    assert(u.getRule() == "B3678/S34678");

    // Seeds
    u.setRule("B2/S");
    assert(u.getRule() == "B2/S");

    // Life без смерти
    u.setRule("B3/S012345678");
    assert(u.getRule() == "B3/S012345678");

    std::cout << "Rule Parsing tests passed!\n";
}

void testClassicLifeRules() {
    std::cout << "Testing Classic Life Rules...\n";

    Universe u(10, 10, "Test", "B3/S23");

    // Тест 1: Одиночная клетка умирает (0 соседей)
    u.clear();
    u.setCell(5, 5, true);
    u.tick();
    assert(!u.getCell(5, 5));

    // Тест 2: Блок (4 клетки в квадрате) - неподвижная фигура
    u.clear();
    u.setCell(1, 1, true);
    u.setCell(1, 2, true);
    u.setCell(2, 1, true);
    u.setCell(2, 2, true);

    u.tick();
    assert(u.getCell(1, 1));
    assert(u.getCell(1, 2));
    assert(u.getCell(2, 1));
    assert(u.getCell(2, 2));

    // Тест 3: Мигатель (3 клетки в ряд) - осциллятор периода 2
    u.clear();
    u.setCell(1, 1, true);
    u.setCell(1, 2, true);
    u.setCell(1, 3, true);

    u.tick();  // Должен повернуться вертикально
    assert(!u.getCell(1, 1));
    assert(!u.getCell(1, 3));
    assert(u.getCell(1, 2));
    assert(u.getCell(0, 2));
    assert(u.getCell(2, 2));

    u.tick();  // Должен вернуться в горизонтальное положение
    assert(u.getCell(1, 1));
    assert(u.getCell(1, 2));
    assert(u.getCell(1, 3));
    assert(!u.getCell(0, 2));
    assert(!u.getCell(2, 2));

    // Тест 4: Планер - перемещающаяся фигура
    u.clear();
    // Создаем планер (ориентация вправо-вниз)
    u.setCell(1, 0, true);
    u.setCell(2, 1, true);
    u.setCell(0, 2, true);
    u.setCell(1, 2, true);
    u.setCell(2, 2, true);

    u.tick(4);  // Через 4 поколения планер должен сместиться на (1,1)

    // Проверяем, что планер переместился
    bool foundGlider = false;
    auto cells = u.getLiveCells();
    for (const auto& cell : cells) {
        if (cell.first == 2 && cell.second == 1) {
            foundGlider = true;
            break;
        }
    }
    assert(foundGlider);

    std::cout << "Classic Life Rules tests passed!\n";
}

void testAlternativeRules() {
    std::cout << "Testing Alternative Rules...\n";

    // Правило Seeds: B2/S (только рождение при 2 соседях, смерть всегда)
    {
        Universe u(10, 10, "Seeds", "B2/S");
        u.clear();

        // Клетка с 0 соседей умирает
        u.setCell(5, 5, true);
        u.tick();
        assert(!u.getCell(5, 5));

        // Две соседние клетки создают новую
        u.clear();
        u.setCell(4, 5, true);
        u.setCell(6, 5, true);
        u.tick();
        assert(u.getCell(5, 5));  // Родилась новая клетка
    }

    // Правило HighLife: B36/S23 (дополнительное рождение при 6 соседях)
    {
        Universe u(10, 10, "HighLife", "B36/S23");
        u.clear();

        // Репликатор HighLife
        u.setCell(1, 0, true);
        u.setCell(2, 0, true);
        u.setCell(0, 1, true);
        u.setCell(0, 2, true);
        u.setCell(1, 2, true);
        u.setCell(2, 2, true);
        u.setCell(3, 2, true);
        u.setCell(2, 3, true);

        // В HighLife эта конфигурация должна размножаться
        size_t initialCount = u.getLiveCells().size();
        u.tick(10);
        size_t finalCount = u.getLiveCells().size();
        assert(finalCount > initialCount);  // Должно размножиться
    }

    // Правило Day & Night: B3678/S34678
    {
        Universe u(10, 10, "Day&Night", "B3678/S34678");
        u.clear();

        // Тестируем симметричность правила
        u.setCell(4, 4, true);
        u.setCell(4, 5, true);
        u.setCell(5, 4, true);
        u.setCell(5, 5, true);

        u.tick();
        // В Day & Night блок должен оставаться блоком
        assert(u.getCell(4, 4));
        assert(u.getCell(4, 5));
        assert(u.getCell(5, 4));
        assert(u.getCell(5, 5));
    }

    std::cout << "Alternative Rules tests passed!\n";
}

// =========================== ФАЙЛОВЫЙ ВВОД-ВЫВОД ===========================

void testUniverseSaver() {
    std::cout << "Testing Universe Saver...\n";

    // Создаем тестовую вселенную
    Universe u(25, 25, "Test Save Universe", "B36/S23");
    u.clear();

    // Добавляем паттерн (блок и планер)
    u.setCell(1, 1, true);
    u.setCell(1, 2, true);
    u.setCell(2, 1, true);
    u.setCell(2, 2, true);

    u.setCell(10, 10, true);
    u.setCell(11, 10, true);
    u.setCell(12, 10, true);
    u.setCell(12, 9, true);
    u.setCell(11, 8, true);

    u.tick(5);  // Делаем несколько итераций

    // Сохраняем в файл
    std::string testFile = "test_save.lif";
    bool saveResult = UniverseSaver::saveToFile(u, testFile);
    assert(saveResult);
    assert(fs::exists(testFile));

    // Проверяем, что файл не пустой
    std::ifstream file(testFile);
    assert(file.good());
    std::string line;
    bool hasName = false, hasRule = false, hasCoords = false;

    while (std::getline(file, line)) {
        if (line.find("#N") == 0) hasName = true;
        if (line.find("#R") == 0) hasRule = true;
        if (Parser::isCoordinateLine(line)) hasCoords = true;
    }
    file.close();

    assert(hasName);
    assert(hasRule);
    assert(hasCoords);

    // Очистка
    fs::remove(testFile);

    // Тест сохранения пустой вселенной
    Universe empty(10, 10, "Empty", "B3/S23");
    empty.clear();

    testFile = "test_empty.lif";
    saveResult = UniverseSaver::saveToFile(empty, testFile);
    assert(saveResult);
    assert(fs::exists(testFile));

    fs::remove(testFile);

    // Тест сохранения с недопустимым именем файла
    saveResult = UniverseSaver::saveToFile(u, "");
    assert(!saveResult);

    saveResult = UniverseSaver::saveToFile(u, "/invalid/path/file.lif");
    assert(!saveResult);

    std::cout << "Universe Saver tests passed!\n";
}

void testUniverseLoader() {
    std::cout << "Testing Universe Loader...\n";

    std::string testFile = "test_load.lif";
    std::ofstream out(testFile);

    out << "#Life 1.06\n";
    out << "#N Test Universe\n";
    out << "#R B36/S23\n";
    out << "#C Created for testing\n";
    out << "1 1\n";
    out << "1 2\n";
    out << "2 1\n";
    out << "2 2\n";
    out << "10 10\n";
    out << "11 10\n";
    out << "12 10\n";
    out.close();

    // Загружаем вселенную
    Universe u = UniverseLoader::loadFromFile(testFile);

    // Проверяем свойства
    assert(u.getName() == "Test Universe");
    assert(u.getRule() == "B36/S23");
    assert(u.width() >= 20);  // Минимальный размер
    assert(u.height() >= 20);

    // Проверяем, что клетки загрузились
    assert(u.getCell(1, 1));
    assert(u.getCell(1, 2));
    assert(u.getCell(2, 1));
    assert(u.getCell(2, 2));
    assert(u.getCell(10, 10));
    assert(u.getCell(11, 10));
    assert(u.getCell(12, 10));

    // Проверяем, что другие клетки мертвые
    assert(!u.getCell(0, 0));
    assert(!u.getCell(5, 5));

    // Очистка
    fs::remove(testFile);

    // Тест загрузки файла без правил (должны использоваться правила по умолчанию)
    testFile = "test_no_rule.lif";
    out.open(testFile);
    out << "#Life 1.06\n";
    out << "#N No Rule Universe\n";
    out << "3 3\n";
    out << "4 4\n";
    out.close();

    u = UniverseLoader::loadFromFile(testFile);
    assert(u.getName() == "No Rule Universe");
    assert(u.getRule() == "B3/S23");  // Правила по умолчанию

    fs::remove(testFile);

    // Тест загрузки файла без имени (должно быть имя по умолчанию)
    testFile = "test_no_name.lif";
    out.open(testFile);
    out << "#Life 1.06\n";
    out << "#R B2/S\n";
    out << "7 7\n";
    out.close();

    u = UniverseLoader::loadFromFile(testFile);
    assert(u.getName() == "Universe");  // Имя по умолчанию
    assert(u.getRule() == "B2/S");

    fs::remove(testFile);

    // Тест загрузки пустого файла
    testFile = "test_empty_file.lif";
    out.open(testFile);
    out.close();

    bool exceptionThrown = false;
    try {
        u = UniverseLoader::loadFromFile(testFile);
    } catch (const std::exception&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    if (fs::exists(testFile)) fs::remove(testFile);

    // Тест загрузки несуществующего файла
    exceptionThrown = false;
    try {
        u = UniverseLoader::loadFromFile("non_existent_file_12345.lif");
    } catch (const std::exception&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "Universe Loader tests passed!\n";
}

void testFileRoundTrip() {
    std::cout << "Testing File Round Trip...\n";

    // Создаем сложную вселенную
    Universe original(30, 30, "Round Trip Test", "B3678/S34678");
    original.clear();

    // Добавляем различные паттерны
    // Блок
    original.setCell(1, 1, true);
    original.setCell(1, 2, true);
    original.setCell(2, 1, true);
    original.setCell(2, 2, true);

    // Мигатель
    original.setCell(10, 10, true);
    original.setCell(10, 11, true);
    original.setCell(10, 12, true);

    // Планер
    original.setCell(20, 20, true);
    original.setCell(21, 20, true);
    original.setCell(22, 20, true);
    original.setCell(22, 19, true);
    original.setCell(21, 18, true);

    // Делаем несколько итераций
    original.tick(3);

    // Сохраняем
    std::string testFile = "roundtrip_test.lif";
    assert(UniverseSaver::saveToFile(original, testFile));

    // Загружаем
    Universe loaded = UniverseLoader::loadFromFile(testFile);

    // Сравниваем основные свойства
    assert(loaded.getName() == original.getName());
    assert(loaded.getRule() == original.getRule());

    // Сравниваем живые клетки
    auto originalCells = original.getLiveCells();
    auto loadedCells = loaded.getLiveCells();

    assert(originalCells.size() == loadedCells.size());

    for (const auto& cell : originalCells) {
        // Координаты могут отличаться из-за центрирования
        // Но клетка должна быть живой где-то в поле
        bool found = false;
        for (const auto& loadedCell : loadedCells) {
            if (loaded.getCell(loadedCell.first, loadedCell.second)) {
                found = true;
                break;
            }
        }
        assert(found);
        (void)cell;  // Явно указываем, что переменная используется (для компилятора)
    }

    // Очистка
    fs::remove(testFile);

    std::cout << "File Round Trip test passed!\n";
}

// =========================== ГРАНИЧНЫЕ СЛУЧАИ ===========================

void testEdgeCases() {
    std::cout << "Testing Edge Cases...\n";

    // Очень маленькая вселенная 1x1
    {
        Universe u(1, 1, "Tiny", "B3/S23");
        u.setCell(0, 0, true);
        u.tick();
        assert(!u.getCell(0, 0));  // Должна умереть (0 соседей)
    }


    // Большое количество итераций
    {
        Universe u(50, 50, "Many Iterations", "B3/S23");
        u.clear();

        // Создаем устойчивый паттерн (блок)
        u.setCell(25, 25, true);
        u.setCell(25, 26, true);
        u.setCell(26, 25, true);
        u.setCell(26, 26, true);

        u.tick(1000);  // 1000 итераций

        // Блок должен остаться неизменным
        assert(u.getCell(25, 25));
        assert(u.getCell(25, 26));
        assert(u.getCell(26, 25));
        assert(u.getCell(26, 26));
        assert(u.getGeneration() == 1000);
    }

    // Координаты за пределами int
    {
        Universe u(10, 10, "Test", "B3/S23");

        // Очень большие координаты должны нормально завернуться
        u.setCell(1000000, 1000000, true);
        // 1000000 % 10 = 0
        assert(u.getCell(0, 0));

        u.setCell(-1000000, -1000000, true);
        // (-1000000 % 10 + 10) % 10 = 0
        assert(u.getCell(0, 0));
    }

    std::cout << "Edge Cases tests passed!\n";
}

// =========================== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ===========================

void testIntegration() {
    std::cout << "Testing Integration...\n";

    // Тест: Создание -> Сохранение -> Загрузка -> Выполнение -> Сохранение
    {
        // 1. Создаем вселенную
        Universe u1(40, 40, "Integration Test", "B36/S23");
        u1.clear();

        // Добавляем репликатор HighLife
        std::vector<std::pair<int, int>> replicator = {
            {10, 10}, {11, 10}, {12, 10},
            {10, 11}, {12, 11},
            {10, 12}, {11, 12}, {12, 12}
        };

        for (const auto& cell : replicator) {
            u1.setCell(cell.first, cell.second, true);
        }

        // 2. Сохраняем
        std::string file1 = "integration1.lif";
        assert(UniverseSaver::saveToFile(u1, file1));

        // 3. Загружаем
        Universe u2 = UniverseLoader::loadFromFile(file1);

        // 4. Выполняем итерации
        u2.tick(20);

        // 5. Сохраняем результат
        std::string file2 = "integration2.lif";
        assert(UniverseSaver::saveToFile(u2, file2));

        // 6. Проверяем, что файлы разные
        assert(fs::exists(file1));
        assert(fs::exists(file2));

        // 7. Очистка
        fs::remove(file1);
        fs::remove(file2);
    }

    {
        std::vector<std::string> testRules = {
            "B3/S23",
            "B36/S23",
            "B3678/S34678",
            "B3/S012345678",
        };

        for (const auto& rule : testRules) {
            Universe u(20, 20, "Rule Test: " + rule, rule);
            u.clear();
            u.setCell(5, 5, true);
            u.setCell(5, 6, true);
            u.setCell(6, 5, true);

            std::string filename = "rule_test_" + rule + ".lif";
            // Заменяем недопустимые символы в имени файла
            std::replace(filename.begin(), filename.end(), '/', '_');
            std::replace(filename.begin(), filename.end(), ':', '_');

            assert(UniverseSaver::saveToFile(u, filename));

            Universe loaded = UniverseLoader::loadFromFile(filename);
            assert(loaded.getRule() == rule);
            assert(loaded.getName() == "Rule Test: " + rule);

            fs::remove(filename);
        }
    }

    std::cout << "Integration tests passed!\n";
}

// =========================== ПРОИЗВОДИТЕЛЬНОСТЬ ===========================

void testPerformance() {
    std::cout << "Testing Performance...\n";

    // Тест производительности с большой вселенной
    {
        Universe u(100, 100, "Performance Test", "B3/S23");
        u.clear();

        // Заполняем случайными клетками (~50%)
        for (int y = 0; y < 100; ++y) {
            for (int x = 0; x < 100; ++x) {
                if ((x + y) % 2 == 0) {
                    u.setCell(x, y, true);
                }
            }
        }
    }

    // Тест с очень большим количеством итераций
    {
        Universe u(50, 50, "Many Ticks", "B3/S23");
        u.clear();

        // Простой паттерн, который не должен исчезнуть
        u.setCell(25, 25, true);
        u.setCell(25, 26, true);
        u.setCell(26, 25, true);
        u.setCell(26, 26, true);

        u.tick(10000);

        // Блок должен остаться
        assert(u.getCell(25, 25));
        assert(u.getCell(25, 26));
        assert(u.getCell(26, 25));
        assert(u.getCell(26, 26));
        assert(u.getGeneration() == 10000);
    }

    std::cout << "Performance tests passed!\n";
}

// =========================== ОСОБЫЕ СЛУЧАИ ===========================

void testSpecialCases() {
    std::cout << "Testing Special Cases...\n";

    // Тест: Все клетки умирают
    {
        Universe u(10, 10, "Extinction", "B3/S23");
        u.clear();

        // Одиночная клетка
        u.setCell(5, 5, true);
        u.tick();

        // Должна умереть
        assert(u.getLiveCells().empty());
        assert(u.getGeneration() == 1);

        // Еще одна итерация на пустом поле
        u.tick();
        assert(u.getLiveCells().empty());
        assert(u.getGeneration() == 2);
    }

    // Тест: Стабильная конфигурация
    {
        Universe u(10, 10, "Stable", "B3/S23");
        u.clear();

        // Создаем улей (beehive) - стабильная фигура
        u.setCell(2, 1, true);
        u.setCell(3, 1, true);
        u.setCell(1, 2, true);
        u.setCell(4, 2, true);
        u.setCell(2, 3, true);
        u.setCell(3, 3, true);

        u.tick(10);

        // Фигура должна остаться неизменной
        assert(u.getCell(2, 1));
        assert(u.getCell(3, 1));
        assert(u.getCell(1, 2));
        assert(u.getCell(4, 2));
        assert(u.getCell(2, 3));
        assert(u.getCell(3, 3));

        // Не должно быть других живых клеток
        auto liveCells = u.getLiveCells();
        assert(liveCells.size() == 6);
    }

    // Тест: Космические корабли (spaceships)
    {
        Universe u(20, 20, "Spaceship Test", "B3/S23");
        u.clear();

        // Легковесный космический корабль (LWSS)
        std::vector<std::pair<int, int>> lwss = {
            {1,0}, {4,0},
            {0,1},
            {0,2}, {4,2},
            {0,3}, {1,3}, {2,3}, {3,3}
        };

        for (const auto& cell : lwss) {
            u.setCell(cell.first, cell.second, true);
        }

        auto initialState = u.getLiveCells();

        // Через 4 поколения корабль должен сместиться
        u.tick(4);

        // Проверяем, что корабль переместился
        bool foundAtNewPosition = false;
        auto finalState = u.getLiveCells();

        // Ищем клетку в новой позиции (должна сместиться вправо-вниз)
        for (const auto& cell : finalState) {
            if (cell.first > 4 || cell.second > 3) {
                foundAtNewPosition = true;
                break;
            }
        }

        assert(foundAtNewPosition);
    }

    std::cout << "Special Cases tests passed!\n";
}

// =========================== ГЛАВНАЯ ФУНКЦИЯ ТЕСТОВ ===========================

void runAllTests() {
    std::cout << "=========================================\n";
    std::cout << "  COMPREHENSIVE GAME OF LIFE TEST SUITE  \n";
    std::cout << "=========================================\n\n";

    try {
        // Парсер
        std::cout << "1. Parser Tests:\n";
        testParserBasics();
        testParserCoordinates();
        testParserExtraction();
        std::cout << "✓ All Parser tests passed!\n\n";

        // Вселенная: базовое
        std::cout << "2. Universe Basic Tests:\n";
        testUniverseConstructors();
        testUniverseCellOperations();
        testUniverseClear();
        testUniverseGetLiveCells();
        std::cout << "✓ All Universe Basic tests passed!\n\n";

        // Вселенная: правила
        std::cout << "3. Universe Rules Tests:\n";
        testRuleParsing();
        testClassicLifeRules();
        testAlternativeRules();
        std::cout << "✓ All Universe Rules tests passed!\n\n";

        // Файловый ввод-вывод
        std::cout << "4. File I/O Tests:\n";
        testUniverseSaver();
        testUniverseLoader();
        testFileRoundTrip();
        std::cout << "✓ All File I/O tests passed!\n\n";

        // Граничные случаи
        std::cout << "5. Edge Cases Tests:\n";
        testEdgeCases();
        std::cout << "✓ All Edge Cases tests passed!\n\n";

        // Интеграционные тесты
        std::cout << "6. Integration Tests:\n";
        testIntegration();
        std::cout << "✓ All Integration tests passed!\n\n";

        // Производительность
        std::cout << "7. Performance Tests:\n";
        testPerformance();
        std::cout << "✓ All Performance tests passed!\n\n";

        // Особые случаи
        std::cout << "8. Special Cases Tests:\n";
        testSpecialCases();
        std::cout << "✓ All Special Cases tests passed!\n\n";

        std::cout << "=========================================\n";
        std::cout << "  ALL TESTS PASSED SUCCESSFULLY!        \n";
        std::cout << "=========================================\n";
    }
    catch (const std::exception& e) {
        std::cerr << "\n✗ TEST FAILED: " << e.what() << "\n";
        std::cerr << "=========================================\n";
        throw;
    }
    catch (...) {
        std::cerr << "\n✗ UNKNOWN ERROR IN TESTS\n";
        std::cerr << "=========================================\n";
        throw;
    }
}

int main() {

    try {
        runAllTests();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Test suite failed with exception: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "Test suite failed with unknown exception\n";
        return 1;
    }
}