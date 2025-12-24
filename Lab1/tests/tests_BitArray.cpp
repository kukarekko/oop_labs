#include <gtest/gtest.h>
#include "BitArray.h"
#include <iostream>
#include <exception>

class BitArrayTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(BitArrayTest, DefaultConstructor) {
    std::cout << "=== TEST 1: DefaultConstructor STARTED ===" << std::endl;

    BitArray a;
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.count(), 0);

    std::cout << "=== TEST 1: DefaultConstructor FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, ConstructorWithSizeAndValue) {
    std::cout << "=== TEST 2: ConstructorWithSizeAndValue STARTED ===" << std::endl;

    BitArray b1(8, 0b10101010);
    std::cout << "BitArray content: " << b1.to_string() << std::endl;
    EXPECT_EQ(b1.size(), 8);
    EXPECT_EQ(b1.count(), 4);
    EXPECT_FALSE(b1.empty());

    BitArray b2(16, 0xFFFF);
    EXPECT_EQ(b2.size(), 16);
    EXPECT_EQ(b2.count(), 16);
    EXPECT_FALSE(b2.empty());

    std::cout << "=== TEST 2: ConstructorWithSizeAndValue FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, CopyBitArray) {
    std::cout << "=== TEST 3: CopyBitArray STARTED ===" << std::endl;

    BitArray a(8, 0b00110011);
    BitArray copy(a);

    EXPECT_EQ(a.size(), copy.size());
    EXPECT_EQ(a.count(), copy.count());
    EXPECT_TRUE(a == copy);

    std::cout << "=== TEST 3: CopyBitArray FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, SimpleOperations) {
    std::cout << "=== TEST 4: SimpleOperations STARTED ===" << std::endl;

    BitArray a(8);
    a.set(0);
    EXPECT_TRUE(a[0]);
    EXPECT_EQ(a.count(), 1);

    a.set(4, true);
    EXPECT_TRUE(a[4]);
    EXPECT_EQ(a.count(), 2);

    a.reset(0);
    EXPECT_FALSE(a[0]);
    EXPECT_EQ(a.count(), 1);

    std::cout << "=== TEST 4: SimpleOperations FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, BasicStringConversion) {
    std::cout << "=== TEST 5: BasicStringConversion STARTED ===" << std::endl;

    BitArray ba(8, 0b10101010);

    EXPECT_EQ(ba.size(), 8);
    EXPECT_EQ(ba.to_string(), "10101010");

    std::cout << "=== TEST 5: BasicStringConversion FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, SwapBitArray) {
    std::cout << "=== TEST 6: SwapBitArray STARTED ===" << std::endl;

    BitArray a(8, 0b01010101);
    BitArray b(8, 0b00000000);
    a.swap(b);

    EXPECT_EQ(a.count(), 0);
    EXPECT_EQ(b.count(), 4);
    EXPECT_TRUE(a.none());
    EXPECT_FALSE(b.none());

    std::cout << "=== TEST 6: SwapBitArray FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, ResizeBitArray) {
    std::cout << "=== TEST 7: ResizeBitArray STARTED ===" << std::endl;

    BitArray a(4, 0b1111);
    EXPECT_EQ(a.size(), 4);
    EXPECT_EQ(a.count(), 4);

    a.resize(8, false);
    EXPECT_EQ(a.size(), 8);
    EXPECT_EQ(a.count(), 4);

    a.resize(6, false);
    EXPECT_EQ(a.size(), 6);
    EXPECT_EQ(a.count(), 4);

    BitArray b;
    b.resize(0, false);
    EXPECT_EQ(b.size(), 0);

    b.resize(40, false);
    EXPECT_EQ(b.size(), 40);
    EXPECT_EQ(b.count(), 0);

    EXPECT_THROW(b.resize(-1), std::invalid_argument);

    BitArray c(5);
    c.resize(10, true);
    EXPECT_EQ(c.size(), 10);
    EXPECT_EQ(c.count(), 5);

    std::cout << "=== TEST 7: ResizeBitArray FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, AssignmentOperator) {
    std::cout << "=== TEST 8: AssignmentOperator STARTED ===" << std::endl;

    BitArray a1(8, 0b10101010);
    BitArray a2;
    a2 = a1;

    EXPECT_TRUE(a1 == a2);
    EXPECT_EQ(a1.size(), a2.size());

    BitArray a3;
    a3 = a1;

    std::cout << "=== TEST 8: AssignmentOperator FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, SetAndReset) {
    std::cout << "=== TEST 9: SetAndReset STARTED ===" << std::endl;

    BitArray a(8);

    a.set(0);
    EXPECT_TRUE(a[0]);
    EXPECT_EQ(a.count(), 1);
    a.set(-1);

    a.set(4, true);
    EXPECT_TRUE(a[4]);
    EXPECT_EQ(a.count(), 2);

    a.reset(0);
    EXPECT_FALSE(a[0]);
    EXPECT_EQ(a.count(), 1);

    a.set();
    EXPECT_EQ(a.count(), 8);

    a.reset();
    EXPECT_EQ(a.count(), 0);
    a.reset(-1);


    std::cout << "=== TEST 9: SetAndReset FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, OperationsAndToString) {
    std::cout << "=== TEST 10: OperationsAndToString STARTED ===" << std::endl;

    BitArray a(8, 0b11001100);
    BitArray b(8, 0b10101010);
    BitArray c(4, 0000);

    EXPECT_THROW(a & c, std::invalid_argument);
    EXPECT_THROW(a | c, std::invalid_argument);
    EXPECT_THROW(a ^ c, std::invalid_argument);

    BitArray res1 = a & b;
    std::cout << "a & b = " << res1.to_string() << std::endl;
    EXPECT_EQ(res1.to_string(), "10001000");

    BitArray res2 = a | b;
    std::cout << "a | b = " << res2.to_string() << std::endl;
    EXPECT_EQ(res2.to_string(), "11101110");

    BitArray res3 = a ^ b;
    std::cout << "a ^ b = " << res3.to_string() << std::endl;
    EXPECT_EQ(res3.to_string(), "01100110");

    BitArray res4 = ~a;
    std::cout << "~a count = " << res4.count() << std::endl;
    EXPECT_EQ(res4.count(), 8 - a.count());

    std::cout << "=== TEST 10: OperationsAndToString FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, ShiftOperations) {
    std::cout << "=== TEST 11: ShiftOperations STARTED ===" << std::endl;

    BitArray a(8, 0b00001111);

    BitArray leftShift = a << 2;
    std::cout << "a << 2 = " << leftShift.to_string() << std::endl;
    EXPECT_EQ(leftShift.to_string(), "00111100");

    BitArray rightShift = a >> 2;
    std::cout << "a >> 2 = " << rightShift.to_string() << std::endl;
    EXPECT_EQ(rightShift.to_string(), "00000011");

    BitArray b = a;
    b <<= 3;
    std::cout << "b <<= 3 = " << b.to_string() << std::endl;
    EXPECT_EQ(b.to_string(), "01111000");

    b <<= 0;
    EXPECT_EQ(b.to_string(), "01111000");
    b >>= 0;
    EXPECT_EQ(b.to_string(), "01111000");

    b <<= 10;
    EXPECT_EQ(b.count(), 0);
    EXPECT_THROW(b <<= -1, std::invalid_argument);
    EXPECT_THROW(b << -1, std::invalid_argument);

    b >>= 10;
    EXPECT_EQ(b.count(), 0);
    EXPECT_THROW(b >>= -1, std::invalid_argument);
    EXPECT_THROW(b >> -1, std::invalid_argument);

    std::cout << "=== TEST 11: ShiftOperations FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, PushBack) {
    std::cout << "=== TEST 12: PushBack STARTED ===" << std::endl;

    BitArray a;

    a.push_back(true);
    EXPECT_EQ(a.size(), 1);
    EXPECT_TRUE(a[0]);

    a.push_back(false);
    EXPECT_EQ(a.size(), 2);
    EXPECT_FALSE(a[1]);

    a.push_back(true);
    EXPECT_EQ(a.size(), 3);
    EXPECT_TRUE(a[2]);

    std::cout << "=== TEST 12: PushBack FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, AnyNone) {
    std::cout << "=== TEST 13: AnyNone STARTED ===" << std::endl;

    BitArray empty(8);
    EXPECT_TRUE(empty.none());
    EXPECT_FALSE(empty.any());

    BitArray some(8);
    some.set(3);
    EXPECT_FALSE(some.none());
    EXPECT_TRUE(some.any());

    BitArray full(8);
    full.set();
    EXPECT_FALSE(full.none());
    EXPECT_TRUE(full.any());

    std::cout << "=== TEST 13: AnyNone FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, Comparison) {
    std::cout << "=== TEST 14: Comparison STARTED ===" << std::endl;

    BitArray a(8, 0b10101010);
    BitArray b(8, 0b10101010);
    BitArray c(8, 0b01010101);
    BitArray d(4, 0b0101);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a == d);
    EXPECT_TRUE(a != d);

    std::cout << "=== TEST 14: Comparison FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, BoundsChecking) {
    std::cout << "=== TEST 15: BoundsChecking STARTED ===" << std::endl;

    BitArray a(8);

    EXPECT_THROW(a[10], std::out_of_range);
    EXPECT_THROW(a[-1], std::out_of_range);

    EXPECT_NO_THROW(a[0]);
    EXPECT_NO_THROW(a[7]);

    std::cout << "=== TEST 15: BoundsChecking FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, LargeBitArray) {
    std::cout << "=== TEST 16: LargeBitArray STARTED ===" << std::endl;

    BitArray a(1000);
    EXPECT_EQ(a.size(), 1000);
    EXPECT_EQ(a.count(), 0);

    a.set(500);
    EXPECT_TRUE(a[500]);
    EXPECT_EQ(a.count(), 1);

    a.set(999);
    EXPECT_TRUE(a[999]);
    EXPECT_EQ(a.count(), 2);

    std::cout << "=== TEST 16: LargeBitArray FINISHED ===" << std::endl;
}

TEST_F(BitArrayTest, ClearBitArray) {
    std::cout << "=== TEST 17: LargeBitArray STARTED ===" << std::endl;
    BitArray a(1000);
    a.clear();
    EXPECT_EQ(a.size(), 0);
    std::cout << "=== TEST 17: LargeBitArray FINISHED ===" << std::endl;
}

