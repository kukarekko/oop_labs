cmake_minimum_required(VERSION 3.10)
project(WordCounter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная программа
add_executable(wordcounter
        src/main.cpp
        src/WordCounter.cpp
        src/CleanerWord.cpp
        src/Reader.cpp
        src/WriterFileCSV.cpp
)

target_include_directories(wordcounter PRIVATE include)

# Простые тесты без GTest
add_executable(test_wordcounter
        tests/test_WordCounter.cpp
        src/WordCounter.cpp
        src/CleanerWord.cpp
        src/Reader.cpp
        src/WriterFileCSV.cpp
)

target_include_directories(test_wordcounter PRIVATE include)