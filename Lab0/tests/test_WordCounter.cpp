#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>
#include "WordCounter.h"
#include "Reader.h"
#include "WriterFileCSV.h"
#include "CleanerWord.h"

void testMainInsufficientArguments() {
    // Запускаем программу без аргументов
    int result = system("wordcounter.exe > nul 2>&1");

    // Проверяем что программа завершилась с ошибкой (код не 0)
    assert(result != 0);
    std::cout << "testMainInsufficientArguments: PASSED\n";
}

void testWordCounterProcess() {
    // Создаем тестовый файл
    std::ofstream testFile("test_process.txt");
    testFile << "hello world hello";
    testFile.close();

    WordCounter wc("test_process.txt", "test_output.csv");
    wc.process(); // Тестируем основной процесс!

    assert(wc.getTotalWords() == 3);
    assert(wc.getWordCounter().at("hello") == 2);
    assert(wc.getWordCounter().at("world") == 1);

    std::remove("test_process.txt");
    std::remove("test_output.csv");
    std::cout << "testWordCounterProcess: PASSED\n";
}

// Тест для пустого входного файла
void testProcessEmptyFile() {
    // Создаем пустой файл
    std::ofstream emptyFile("empty_test.txt");
    emptyFile.close();

    WordCounter wc("empty_test.txt", "empty_output.csv");
    wc.process(); // Должен вывести сообщение о пустом файле

    // Проверяем что счетчики остались нулевыми
    assert(wc.getTotalWords() == 0);
    assert(wc.getWordCounter().empty());

    std::remove("empty_test.txt");
    std::cout << "testProcessEmptyFile: PASSED\n";
}

// Тест для несуществующего входного файла
void testProcessNonExistentFile() {
    WordCounter wc("non_existent_input.txt", "output.csv");
    wc.process();

    assert(wc.getTotalWords() == 0);
    assert(wc.getWordCounter().empty());
    std::cout << "testProcessNonExistentFile: PASSED\n";
}

// Тест для ошибки записи выходного файла
void testProcessWriteError() {
    std::ofstream testFile("write_test.txt");
    testFile << "hello world test";
    testFile.close();

    WordCounter wc("write_test.txt", "C:/Windows/system32/forbidden_output.csv");
    wc.process();
    assert(wc.getTotalWords() == 3);
    assert(wc.getWordCounter().at("hello") == 1);
    assert(wc.getWordCounter().at("world") == 1);
    assert(wc.getWordCounter().at("test") == 1);

    std::remove("write_test.txt");
    std::cout << "testProcessWriteError: PASSED\n";
}

// Тест для недоступной директории (альтернативный вариант)
void testProcessInvalidOutputPath() {
    std::ofstream testFile("invalid_path_test.txt");
    testFile << "simple test";
    testFile.close();

    WordCounter wc("invalid_path_test.txt", "invalid/path/output.csv");
    wc.process();

    assert(wc.getTotalWords() == 2);

    std::remove("invalid_path_test.txt");
    std::cout << "testProcessInvalidOutputPath: PASSED\n";
}

// Тест для Reader с ошибкой
void testReaderWithError() {
    std::vector<std::string> lines = readLines("non_existent_file.txt");
    assert(lines.empty());
    std::cout << "testReaderWithError: PASSED\n";
}

// Тест для Writer
void testWriterCSV() {
    std::vector<std::pair<std::string, int>> words = {
        {"hello", 3},
        {"world", 1}
    };

    bool result = writeOutput("test_writer.csv", words, 4);
    assert(result == true);

    // Проверяем что файл создался
    std::ifstream file("test_writer.csv");
    assert(file.is_open());
    file.close();

    std::remove("test_writer.csv");
    std::cout << "testWriterCSV: PASSED\n";
}

void testCountWordsEmpty() {
    WordCounter wc("input.txt", "output.csv");
    wc.resetCounter();
    std::vector<std::string> lines = {""};
    wc.testCountWords(lines);

    assert(wc.getTotalWords() == 0);
    assert(wc.getWordCounter().empty());
    std::cout << "testCountWordsEmpty: PASSED\n";
}

void testCountWordsBasic() {
    WordCounter wc("input.txt", "output.csv");
    wc.resetCounter();
    std::vector<std::string> lines = {"hello world", "world hello"};
    wc.testCountWords(lines);

    assert(wc.getTotalWords() == 4);
    assert(wc.getWordCounter().at("hello") == 2);
    assert(wc.getWordCounter().at("world") == 2);
    std::cout << "testCountWordsBasic: PASSED\n";
}

void testCountWordsCaseInsensitivity() {
    WordCounter wc("input.txt", "output.csv");
    wc.resetCounter();
    std::vector<std::string> lines = {"Hello hello HeLLo hELLo"};
    wc.testCountWords(lines);

    assert(wc.getTotalWords() == 4);
    assert(wc.getWordCounter().at("hello") == 4);
    std::cout << "testCountWordsCaseInsensitivity: PASSED\n";
}

void testCountWordsMultipleLines() {
    WordCounter wc("input.txt", "output.csv");
    wc.resetCounter();
    std::vector<std::string> lines = {"hello test hello Hello test", "World hello"};
    wc.testCountWords(lines);

    assert(wc.getTotalWords() == 7);
    assert(wc.getWordCounter().at("test") == 2);
    assert(wc.getWordCounter().at("hello") == 4);
    assert(wc.getWordCounter().at("world") == 1);
    std::cout << "testCountWordsMultipleLines: PASSED\n";
}

void testCountWordsSymbolsOnly() {
    WordCounter wc("input.txt", "output.csv");
    wc.resetCounter();
    std::vector<std::string> lines = {"!!!", "@@@"};
    wc.testCountWords(lines);

    assert(wc.getTotalWords() == 0);
    assert(wc.getWordCounter().empty());
    std::cout << "testCountWordsSymbolsOnly: PASSED\n";
}

void testCountWordsWithPunctuation() {
    WordCounter wc("input.txt", "output.csv");
    wc.resetCounter();
    std::vector<std::string> lines = {"hello, world! hello; world?"};
    wc.testCountWords(lines);

    assert(wc.getTotalWords() == 4);
    assert(wc.getWordCounter().at("hello") == 2);
    assert(wc.getWordCounter().at("world") == 2);
    std::cout << "testCountWordsWithPunctuation: PASSED\n";
}

void testWriterCreatesFile() {
    std::vector<std::pair<std::string, int>> words = {{"test", 1}};
    bool result = writeOutput("test_output.csv", words, 1);
    assert(result == true);
    std::remove("test_output.csv");
    std::cout << "testWriterCreatesFile(): PASSED\n";
}

void testReaderWithRealFile() {
    std::ofstream testFile("test.txt");
    testFile << "line1\nline2";
    testFile.close();
    auto lines = readLines("test.txt");
    assert(lines.size() == 2);
    std::remove("test.txt");
    std::cout << "testReaderWithRealFile(): PASSED\n";
}

void testMainCorrectArguments() {
    // Создаем тестовый файл
    std::ofstream testFile("test_main_input.txt");
    testFile << "hello world test";
    testFile.close();

    // Запускаем программу с правильными аргументами
    int result = system("wordcounter.exe test_main_input.txt test_main_output.csv");

    // Проверяем что программа завершилась успешно (код 0)
    assert(result == 0);

    // Проверяем что выходной файл создался
    std::ifstream output("test_main_output.csv");
    assert(output.is_open());

    // Проверяем содержимое файла
    std::string line;
    std::getline(output, line);
    assert(line.find("hello") != std::string::npos);
    output.close();

    // Убираем за собой
    std::remove("test_main_input.txt");
    std::remove("test_main_output.csv");

    std::cout << "testMainCorrectArguments: PASSED\n";
}

int main() {
    try {
        std::cout << "Running WordCounter tests...\n";

        testMainCorrectArguments();
        testMainInsufficientArguments();
        testCountWordsEmpty();
        testCountWordsBasic();
        testCountWordsCaseInsensitivity();
        testCountWordsMultipleLines();
        testCountWordsSymbolsOnly();
        testCountWordsWithPunctuation();

        testProcessEmptyFile();
        testProcessNonExistentFile();
        testProcessWriteError();
        testProcessInvalidOutputPath();

        testWordCounterProcess();
        testReaderWithError();
        testWriterCSV();
        testReaderWithRealFile();
        testWriterCreatesFile();

        std::cout << "\nAll tests PASSED!\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Test FAILED: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown test FAILED\n";
        return 1;
    }
}