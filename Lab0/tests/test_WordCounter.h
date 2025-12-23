#include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_WordCounter
    test_WordCounter.cpp
)

target_link_libraries(test_WordCounter
    PRIVATE
    wordcounter_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_WordCounter)dif