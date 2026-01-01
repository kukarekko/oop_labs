cmake_minimum_required(VERSION 3.10)
project(LifeGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Сначала определяем компилятор
if(MSVC)
    add_compile_options(/W4 /WX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    # Для MinGW/GCC
    add_compile_options(-Wall -Wextra -Wpedantic
            -Wno-unused-parameter -Wno-sign-compare)

    # Добавляем отладку и статическую линковку для MinGW
    if(MINGW)
        # Статическая линковка библиотек для MinGW, чтобы не зависеть от DLL
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Основные исходники
set(SOURCES
        Utils/src/Parser.cpp
        Utils/src/FileReader.cpp
        Utils/src/FileWriter.cpp
        Universe/src/Universe.cpp
        Universe/src/UniverseLoader.cpp
        Universe/src/UniverseSaver.cpp
        GameInterface/src/GameInterface.cpp
        main.cpp
)

set(HEADERS
        Utils/include/Parser.h
        Utils/include/FileReader.h
        Utils/include/FileWriter.h
        Universe/include/Universe.h
        Universe/include/UniverseLoader.h
        Universe/include/UniverseSaver.h
        GameInterface/include/GameInterface.h
)

# Основная программа
add_executable(LifeGame ${SOURCES})
target_include_directories(LifeGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Для Windows: настройка UTF-8
if(WIN32)
    target_compile_definitions(LifeGame PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Исходники для тестов (БЕЗ main.cpp и GameInterface.cpp)
set(TEST_SOURCES
        Utils/src/Parser.cpp
        Utils/src/FileReader.cpp
        Utils/src/FileWriter.cpp
        Universe/src/Universe.cpp
        Universe/src/UniverseLoader.cpp
        Universe/src/UniverseSaver.cpp
)

# Тестовая программа
add_executable(LifeGameTests
        tests.cpp
        ${TEST_SOURCES}
)
target_include_directories(LifeGameTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Для Windows: настройка UTF-8 для тестов
if(WIN32)
    target_compile_definitions(LifeGameTests PRIVATE _CRT_SECURE_NO_WARNINGS)

    # Для MinGW: статическая линковка
    if(MINGW)
        set_target_properties(LifeGameTests PROPERTIES
                LINK_FLAGS "-static"
        )
    endif()
endif()

# Добавляем тестовую цель
add_custom_target(run_tests
        COMMAND cmd /c "echo Running tests..." && ${CMAKE_CURRENT_BINARY_DIR}/LifeGameTests
        DEPENDS LifeGameTests
        COMMENT "Running unit tests..."
)

# Простая цель для сборки тестов
add_custom_target(build_tests
        DEPENDS LifeGameTests
        COMMENT "Building tests..."
)

# Установка
install(TARGETS LifeGame
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(FILES ${HEADERS} DESTINATION include/LifeGame)

# Проверка наличия заголовочных файлов
foreach(header ${HEADERS})
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${header})
        message(WARNING "Header file ${header} not found!")
    endif()
endforeach()

# Информационные сообщения
message(STATUS "Project: LifeGame")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

# Проверка версии компилятора
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        message(WARNING "GCC version ${CMAKE_CXX_COMPILER_VERSION} may not fully support C++17")
    endif()
    message(STATUS "GCC version: ${CMAKE_CXX_COMPILER_VERSION}")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        message(WARNING "Clang version ${CMAKE_CXX_COMPILER_VERSION} may not fully support C++17")
    endif()
elseif(MSVC)
    if(MSVC_VERSION LESS 1910)  # Visual Studio 2017
        message(WARNING "MSVC version may not fully support C++17")
    endif()
endif()