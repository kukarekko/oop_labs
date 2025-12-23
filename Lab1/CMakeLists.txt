cmake_minimum_required(VERSION 3.16)
project(Lab1)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

add_executable(Lab1_App
        src/main.cpp
        src/BitArray.cpp
)

add_executable(Lab1_Tests
        tests/tests_BitArray.cpp
        src/BitArray.cpp
)

target_include_directories(Lab1_App PRIVATE include)
target_include_directories(Lab1_Tests PRIVATE include)

target_link_libraries(Lab1_Tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Lab1_Tests)

if(TARGET Lab1_Tests)
    message(STATUS "Lab1_Tests target found - tests are available")
else()
    message(WARNING "Lab1_Tests target not found")
endif()


