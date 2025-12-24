#ifndef BITARRAY_H
#define BITARRAY_H

#include <string>
#include <stdexcept>
#include <algorithm>
#include <climits>

class BitArray {
private:
    unsigned long* array;
    int bitSize;
    int bitCapacity;
    
    static const int BITS_PER_LONG = sizeof(unsigned long) * CHAR_BIT;
    
    int calculateBlocks(int bits) const {
        return (bits + BITS_PER_LONG - 1) / BITS_PER_LONG;
    }

    void resizeCapacity(int newCapacity);

public:
    BitArray();
    explicit BitArray(int num_bits, unsigned long value = 0);
    BitArray(const BitArray& b);
    ~BitArray();

    void swap(BitArray& b);
    BitArray& operator=(const BitArray& b);
    void resize(int numBits, bool value = false);
    void clear();
    void push_back(bool bit);

    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);
    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;

    BitArray& set(int n, bool val = true);
    BitArray& set();
    BitArray& reset(int n);
    BitArray& reset();

    bool any() const;
    bool none() const;
    BitArray operator~() const;
    int count() const;

    bool operator[](int i) const;
    int size() const;
    bool empty() const;
    std::string to_string() const;
};

bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);
BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);

#endif
