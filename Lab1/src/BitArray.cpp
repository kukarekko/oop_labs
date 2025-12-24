#include <cstring>
#include <iostream>
#include "BitArray.h"

BitArray::BitArray() : array(nullptr), bitSize(0), bitCapacity(0) {}

BitArray::~BitArray() { delete[] array; }

BitArray::BitArray(int num_bits, unsigned long value)
    : array(nullptr), bitSize(0), bitCapacity(0) {
    if (num_bits > 0) {
        int blocks = calculateBlocks(num_bits);
        array = new unsigned long[blocks]();
        bitSize = num_bits;
        bitCapacity = blocks * BITS_PER_LONG;
        if (num_bits <= BITS_PER_LONG)
            array[0] = value & ((1UL << num_bits) - 1);
        else {
            array[0] = value;
            for (int i = 1; i < blocks; i++) array[i] = 0;
        }
    }
}

BitArray::BitArray(const BitArray& b)
    : array(nullptr), bitSize(b.bitSize), bitCapacity(b.bitCapacity) {
    if (bitCapacity > 0) {
        int blocks = calculateBlocks(bitCapacity);
        array = new unsigned long[blocks];
        std::copy(b.array, b.array + blocks, array);
    }
}

void BitArray::swap(BitArray& b) {
    std::swap(array, b.array);
    std::swap(bitSize, b.bitSize);
    std::swap(bitCapacity, b.bitCapacity);
}

void BitArray::resize(int numBits, bool value) {
    if (numBits < 0) throw std::invalid_argument("Negative size not allowed");

    if (bitSize == numBits) return;

    int oldSize = bitSize;
    if (numBits > bitCapacity) {
        int newCapacity;
        if (bitCapacity == 0)
            newCapacity = numBits;
        else
            newCapacity = std::max(numBits, bitCapacity * 2);

        resizeCapacity(newCapacity);
    }
    bitSize = numBits;
    if (numBits > oldSize) {
        for (int i = oldSize; i < numBits; i++) set(i, value);
    }
}

void BitArray::resizeCapacity(int newCapacity) {
    if (newCapacity <= bitCapacity) return;
    int newBlocks = calculateBlocks(newCapacity);
    unsigned long* new_array = new unsigned long[newBlocks]();
    array = new_array;
    bitCapacity = newCapacity;
}

BitArray& BitArray::operator=(const BitArray& other) {
    if (this == &other) return *this;
    BitArray temp = other;
    swap(temp);
    return *this;
}

void BitArray::clear() {
    delete[] array;
    array = nullptr;
    bitSize = 0;
    bitCapacity = 0;
}

void BitArray::push_back(bool bit) {
    if (bitSize + 1 > bitCapacity) {
        int newCapacity = (bitCapacity == 0) ? 32 : bitCapacity * 2;
        resizeCapacity(newCapacity);
    }
    bitSize++;
    set(bitSize - 1, bit);
}

BitArray& BitArray::operator&=(const BitArray& b) {
    if (bitSize != b.bitSize) throw std::invalid_argument("Size mismatch");
    int blocks = calculateBlocks(bitSize);
    for (int i = 0; i < blocks; i++) {
        array[i] &= b.array[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    if (bitSize != b.bitSize) throw std::invalid_argument("Size mismatch");
    int blocks = calculateBlocks(bitSize);
    for (int i = 0; i < blocks; i++) {
        array[i] |= b.array[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    if (bitSize != b.bitSize) throw std::invalid_argument("Size mismatch");
    int blocks = calculateBlocks(bitSize);
    for (int i = 0; i < blocks; i++) {
        array[i] ^= b.array[i];
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    if (n < 0) throw std::invalid_argument("Negative shift not allowed");
    if (n == 0) return *this;

    if (n >= bitSize) {
        reset();
        return *this;
    }
    for (int i = bitSize - 1; i >= n; i--) {
        set(i, (*this)[i - n]);
    }
    for (int i = 0; i < n; i++) {
        reset(i);
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n < 0) throw std::invalid_argument("Negative shift not allowed");
    if (n == 0) return *this;
    if (n >= bitSize) {
        reset();
        return *this;
    }
    for (int i = 0; i < bitSize - n; i++) {
        set(i, (*this)[i + n]);
    }
    for (int i = bitSize - 1; i < bitSize; i++) {
        reset(i);
    }
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    BitArray b = *this;
    b <<= n;
    return b;
}

BitArray BitArray::operator>>(int n) const {
    BitArray b = *this;
    b >>= n;
    return b;
}

BitArray& BitArray::set(int n, bool val) {
    if (n < 0 || n >= bitSize) return *this;
    int blockIndex = n / BITS_PER_LONG;
    int bitIndex = n % BITS_PER_LONG;
    if (val) {
        array[blockIndex] |= (1UL << bitIndex);
    } else {
        array[blockIndex] &= ~(1UL << bitIndex);
    }
    return *this;
}

BitArray& BitArray::set() {
    for (int i = 0; i < bitSize; i++) set(i, true);
    return *this;
}

BitArray& BitArray::reset(int n) {
    if (n < 0 || n >= bitSize) return *this;
    int blockIndex = n / BITS_PER_LONG;
    int bitIndex = n % BITS_PER_LONG;
    array[blockIndex] &= ~(1UL << bitIndex);
    return *this;
}

BitArray& BitArray::reset() {
    for (int i = 0; i < bitSize; i++) reset(i);
    return *this;
}

bool BitArray::any() const {
    int blocks = calculateBlocks(bitSize);
    for (int i = 0; i < blocks; i++) {
        if (array[i] > 0) return true;
    }
    return false;
}

bool BitArray::none() const {
    int blocks = calculateBlocks(bitSize);
    for (int i = 0; i < blocks; i++) {
        if (array[i] > 0) return false;
    }
    return true;
}

BitArray BitArray::operator~() const {
    BitArray b = *this;
    int blocks = calculateBlocks(bitSize);
    for (int i = 0; i < blocks; i++) {
        b.array[i] = ~array[i];
    }
    return b;
}

bool BitArray::operator[](int i) const {
    if (i < 0 || i >= bitSize)
        throw std::out_of_range("BitArray index out of range");
    int blockIndex = i / BITS_PER_LONG;
    int bitIndex = i % BITS_PER_LONG;
    return ((array[blockIndex] >> bitIndex) & 1);
}

int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < bitSize; i++) {
        if ((*this)[i]) count++;
    }
    return count;
}

int BitArray::size() const { return bitSize; }

bool BitArray::empty() const { return bitSize == 0; }

std::string BitArray::to_string() const {
    std::string str;
    for (int i = bitSize - 1; i >= 0; i--) {
        if ((*this)[i])
            str += "1";
        else
            str += "0";
    }
    return str;
}

bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) return false;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) { return !(a == b); }

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    if (b1.size() != b2.size()) throw std::invalid_argument("Size mismatch");
    BitArray a = b1;
    a &= b2;
    return a;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    if (b1.size() != b2.size()) throw std::invalid_argument("Size mismatch");
    BitArray a = b1;
    a |= b2;
    return a;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    if (b1.size() != b2.size()) std::cerr << "Size mismatch";
    BitArray a = b1;
    a ^= b2;
    return a;
}

