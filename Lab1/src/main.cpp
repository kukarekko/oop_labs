#include "BitArray.h"
#include <iostream>
#include <bitset>

int main() {
    std::cout << "=== DEBUG BITARRAY TEST ===" << std::endl;

    // Тест 1: Пустой BitArray
    std::cout << "\n1. Testing empty BitArray:" << std::endl;
    try {
        BitArray empty;
        std::cout << "   empty.size() = " << empty.size() << std::endl;
        std::cout << "   empty.empty() = " << empty.empty() << std::endl;
        std::cout << "   empty.count() = " << empty.count() << std::endl;
        std::cout << "   SUCCESS: Empty BitArray works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   ERROR: " << e.what() << std::endl;
    }

    // Тест 2: BitArray с размером
    std::cout << "\n2. Testing BitArray with size:" << std::endl;
    try {
        BitArray ba1(8);
        std::cout << "   ba1.size() = " << ba1.size() << std::endl;
        std::cout << "   ba1.empty() = " << ba1.empty() << std::endl;
        std::cout << "   ba1.count() = " << ba1.count() << std::endl;
        std::cout << "   ba1.to_string() = " << ba1.to_string() << std::endl;
        std::cout << "   SUCCESS: Sized BitArray works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   ERROR: " << e.what() << std::endl;
    }

    // Тест 3: BitArray со значением
    std::cout << "\n3. Testing BitArray with value (0b10101010):" << std::endl;
    try {
        BitArray ba2(8, 0b10101010);
        std::cout << "   ba2.size() = " << ba2.size() << std::endl;
        std::cout << "   ba2.count() = " << ba2.count() << std::endl;
        std::cout << "   ba2.to_string() = " << ba2.to_string() << std::endl;

        // Проверим отдельные биты
        std::cout << "   Checking bits: ";
        for (int i = 0; i < ba2.size(); i++) {
            std::cout << ba2[i];
        }
        std::cout << std::endl;

        // Сравним с std::bitset
        std::bitset<8> bs(0b10101010);
        std::cout << "   std::bitset: " << bs << std::endl;

        std::cout << "   SUCCESS: BitArray with value works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   ERROR: " << e.what() << std::endl;
    }

    // Тест 4: Проверка any() и none()
    std::cout << "\n4. Testing any() and none():" << std::endl;
    try {
        BitArray empty(8);
        BitArray with_bits(8, 0b00010000);

        std::cout << "   empty.any() = " << empty.any() << std::endl;
        std::cout << "   empty.none() = " << empty.none() << std::endl;
        std::cout << "   with_bits.any() = " << with_bits.any() << std::endl;
        std::cout << "   with_bits.none() = " << with_bits.none() << std::endl;

        std::cout << "   SUCCESS: any()/none() work" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   ERROR: " << e.what() << std::endl;
    }

    std::cout << "\n=== DEBUG TEST COMPLETED ===" << std::endl;
    return 0;
}

